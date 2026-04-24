#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <widget.h>
#include <surface.h>
#include <platform.h>
#include <render.h>
#include <log.h>

static tgui_list_t surfaces;

TOBJECT_DEFINE_CLASS(tgui_surface, TGUI_SURFACE, tgui_widget_get_type())

static void tgui_surface_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_surface_t *surface = TGUI_SURFACE_CAST(widget);
	if (surface->child == child) {
		surface->child = NULL;
	}
}

static int tgui_surface_constructor(void *object) {
	tgui_surface_get_parent_class()->constructor(object);

	tgui_surface_t *surface = TGUI_SURFACE_CAST(object);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(surface), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(surface), TGUI_TRUE);
	surface->scaling = 1;

	// TODO : do more init here

	return 0;
}

static int tgui_surface_destructor(void *object) {
	tgui_surface_t *surface = TGUI_SURFACE_CAST(object);
	tgui_surface_unregister(surface);
	tgui_platform_close_surface(surface);

	return tgui_surface_get_parent_class()->destructor(object);
}

static void tgui_surface_class_init(tgui_surface_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_container_single_calculate_sizes;
	widget_class->allocate_space  = tgui_container_single_allocate_space;
	widget_class->remove_child    = tgui_surface_remove_child;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_surface_constructor;
	tobject_class->destructor  = tgui_surface_destructor;
}

static void tgui_surface_reset_dirty(tgui_surface_t *surface) {
	surface->inval.start_x = LONG_MAX;
	surface->inval.start_y = LONG_MAX;
	surface->inval.end_x = 0;
	surface->inval.end_y = 0;
}

tgui_surface_t *tgui_surface_new(long width, long height, tgui_surface_t *parent) {
	tgui_surface_t *surface = tobject_new(tgui_surface_get_type());
	if (!surface) return NULL;

	// TODO : move this to constructor
	surface->width  = width;
	surface->height = height;

	tgui_platform_create_surface(surface, parent);
	tgui_surface_register(surface);

	tgui_rect_t rect;
	tgui_rect_init(&rect, 0, 0, width, height);
	tgui_surface_invalidate(surface, &rect);
	return surface;
}

void tgui_surface_set_child(tgui_surface_t *surface, tgui_widget_t *child) {
	// if we aready have a child destroy it
	if (surface->child) {
		tgui_widget_destroy(surface->child);
	}
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(surface));
	surface->child = child;
}

tgui_widget_t *tgui_surface_get_child(tgui_surface_t *surface) {
	return surface->child;
}

int tgui_surface_resize(tgui_surface_t *surface, long width, long height) {
	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(surface));
	if (width < surface->widget.min_width || width < surface->widget.min_height) {
		// won't do it
		// too small
		return -1;
	}
	surface->width = width;
	surface->height = height;

	// now widgets could get more or less space
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(surface));
	return 0;
}

static int tgui_surface_is_dirty(tgui_surface_t *surface) {
	return surface->inval.start_x != LONG_MAX;
}

void tgui_surface_render(tgui_surface_t *surface) {
	// update show/hide
	if ((surface->widget.flags & TGUI_WIDGET_HIDDEN) != (surface->old_flags & TGUI_WIDGET_HIDDEN)) {
		tgui_platform_set_surface_visible(surface, !tgui_widget_is_hidden(TGUI_WIDGET_CAST(surface)));
	}
	surface->old_flags = surface->widget.flags;

	if (tgui_widget_is_hidden(TGUI_WIDGET_CAST(surface))) {
		return;
	}

	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(surface));
	tgui_widget_allocate_space(TGUI_WIDGET_CAST(surface), 0, 0, surface->width / surface->scaling, surface->height / surface->scaling);

	if (tgui_surface_is_dirty(surface)) {
		tgui_log("got dirty rect from %ld %ld to %ld %ld\n", surface->inval.start_x, surface->inval.start_y, surface->inval.end_x, surface->inval.end_y);

		// keep the dirty in bounds
		tgui_rect_t bounds;
		tgui_rect_init(&bounds, 0, 0, TGUI_WIDGET_CAST(surface)->width, TGUI_WIDGET_CAST(surface)->height);
		tgui_rect_intersect(&surface->inval, &surface->inval, &bounds);
		tgui_render_set_clip(surface, &surface->inval);
		tgui_widget_render(TGUI_WIDGET_CAST(surface));
		tgui_platform_push_surface(surface);
	}
	tgui_surface_reset_dirty(surface);
}

tgui_list_t *tgui_get_surfaces(void) {
	return &surfaces;
}

void tgui_surface_set_scaling(tgui_surface_t *surface, long scaling) {
	surface->scaling = scaling;
	tgui_widget_mark_dirty_space(tgui_surface_get_child(surface));
}

long tgui_surface_get_scaling(tgui_surface_t *surface) {
	if (!surface) return 1;
	return surface->scaling;
}

void tgui_surface_set_position(tgui_surface_t *surface, long x, long y) {
	tgui_platform_set_surface_position(surface, x, y);
}

void tgui_surface_set_focus(tgui_surface_t *surface, tgui_widget_t *widget) {
	surface->focus = widget;
}

tgui_widget_t *tgui_surface_get_focus(tgui_surface_t *surface) {
	if (!surface) return NULL;
	return surface->focus;
}

void tgui_surface_invalidate(tgui_surface_t *surface, tgui_rect_t *rect) {
	if (!surface) return;
	if (rect->start_x >= rect->end_x) return;
	if (rect->start_y >= rect->end_y) return;
	if (rect->start_x < surface->inval.start_x) {
		surface->inval.start_x = rect->start_x;
	}
	if (rect->start_y < surface->inval.start_y) {
		surface->inval.start_y = rect->start_y;
	}
	if (rect->end_x > surface->inval.end_x) {
		surface->inval.end_x = rect->end_x;
	}
	if (rect->end_y > surface->inval.end_y) {
		surface->inval.end_y = rect->end_y;
	}
}

void tgui_surface_register(tgui_surface_t *surface) {
	tgui_list_append(&surfaces, &surface->node);
}

void tgui_surface_unregister(tgui_surface_t *surface) {
	tgui_list_remove(&surfaces, &surface->node);
}
