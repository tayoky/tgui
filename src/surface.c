#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <widget.h>
#include <surface.h>
#include <platform.h>

static tgui_list_t surfaces;

void tgui_surface_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_surface_t *surface = TGUI_SURFACE_CAST(widget);
	if (surface->child == child) {
		surface->child = NULL;
	}
}

static void tgui_surface_free(tgui_widget_t *widget) {
	tgui_surface_t *surface = TGUI_SURFACE_CAST(widget);
	tgui_surface_unregister(surface);
	tgui_platform_close_surface(surface);
}

static tgui_widget_class_t surface_class = {
	.name = "surface",
	.size = sizeof(tgui_surface_t),
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space  = tgui_container_single_allocate_space,
	.remove_child = tgui_surface_remove_child,
	.free = tgui_surface_free,
};

static void tgui_surface_reset_dirty(tgui_surface_t *surface) {
	surface->inval_start_x = LONG_MAX;
	surface->inval_start_y = LONG_MAX;
	surface->inval_end_x = 0;
	surface->inval_end_y = 0;
}

tgui_surface_t *tgui_surface_new(long width, long height, tgui_surface_t *parent) {
	tgui_widget_t *widget = tgui_widget_new(&surface_class);
	if (!widget) return NULL;


	tgui_surface_t *surface = TGUI_SURFACE_CAST(widget);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(surface), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(surface), TGUI_TRUE);
	surface->width  = width;
	surface->height = height;
	surface->scaling = 1;

	tgui_platform_create_surface(surface, parent);
	tgui_surface_register(surface);
	tgui_surface_invalidate(surface, 0, 0, width, height);
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
	return surface->inval_start_x != LONG_MAX;
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
		printf("got dirty rect from %ld %ld to %ld %ld\n", surface->inval_start_x, surface->inval_start_y, surface->inval_end_x, surface->inval_end_y);
		tgui_platform_set_clip(surface, surface->inval_start_x, surface->inval_start_y, 
		(surface->inval_end_x - surface->inval_start_x) * surface->scaling, 
		(surface->inval_end_y - surface->inval_start_y) * surface->scaling);
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
	return surface->scaling;
}

void tgui_surface_set_position(tgui_surface_t *surface, long x, long y) {
	tgui_platform_set_surface_position(surface, x, y);
}

void tgui_surface_set_focus(tgui_surface_t *surface, tgui_widget_t *widget) {
	surface->focus = widget;
}

tgui_widget_t *tgui_surface_get_focus(tgui_surface_t *surface) {
	return surface->focus;
}

void tgui_surface_invalidate(tgui_surface_t *surface, long x, long y, long width, long height) {
	if (width == 0 || height == 0) return;
	long end_x = x + width;
	long end_y = y + height;
	if (x < surface->inval_start_x) {
		surface->inval_start_x = x;
	}
	if (y < surface->inval_start_y) {
		surface->inval_start_y = y;
	}
	if (end_x > surface->inval_end_x) {
		surface->inval_end_x = end_x;
	}
	if (end_y > surface->inval_end_y) {
		surface->inval_end_y = end_y;
	}
}

void tgui_surface_register(tgui_surface_t *surface) {
	tgui_list_append(&surfaces, &surface->node);
}

void tgui_surface_unregister(tgui_surface_t *surface) {
	tgui_list_remove(&surfaces, &surface->node);
}
