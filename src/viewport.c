#include <viewport.h>
#include <render.h>
#include <common.h>

TOBJECT_DEFINE_CLASS(tgui_viewport, TGUI_VIEWPORT, tgui_widget_get_type())

static void tgui_viewport_calculate_sizes(tgui_widget_t *widget) {
	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(widget);

	// min size 10x10 to prevent colapse
	long min_width;
	if (viewport->have_hscroll) {
		min_width = 10;
	} else {
		min_width = tgui_viewport_get_content_min_width(viewport);
	}

	long min_height;
	if (viewport->have_vscroll) {
		min_height = 10;
	} else {
		min_height = tgui_viewport_get_content_min_height(viewport);
	}

	widget->min_width  = min_width;
	widget->min_height = min_height;
	widget->pref_width  = tgui_viewport_get_content_pref_width(viewport);
	widget->pref_height = tgui_viewport_get_content_pref_height(viewport);
}

static void tgui_viewport_allocate_space(tgui_widget_t *widget) {
	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(widget);

	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);
	long content_width  = tgui_viewport_get_content_pref_width(viewport);
	long content_height = tgui_viewport_get_content_pref_height(viewport);

	if (viewport->have_hscroll) {
		width = max(width, content_width);
	}
	if (viewport->have_vscroll) {
		height = max(height, content_height);
	}

	tgui_widget_allocate_space(viewport->child, x - viewport->scroll_x, y - viewport->scroll_y, width, height);
}

static void tgui_viewport_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(widget);
	if (viewport->child == child) {
		viewport->child = NULL;
	}
}

static void tgui_viewport_render(tgui_widget_t *widget) {
	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(widget);
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	tgui_render_get_clip(surface, &viewport->old_clip);
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	tgui_rect_t viewport_clip;
	tgui_rect_init(&viewport_clip, x, y, width, height);
	tgui_rect_intersect(&viewport_clip, &viewport_clip, &viewport->old_clip);

	// if the viewport is totaly offscreen
	// we can save time
	if (viewport_clip.start_x >= viewport_clip.end_x) return;
	if (viewport_clip.start_y >= viewport_clip.end_y) return;

	tgui_render_set_clip(surface, &viewport_clip);
}

static void tgui_viewport_after_render(tgui_widget_t *widget) {
	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(widget);
	tgui_surface_t *surface = tgui_widget_get_surface(widget);

	tgui_render_set_clip(surface, &viewport->old_clip); 
}

static int tgui_viewport_constructor(void *object) {
	tgui_viewport_get_parent_class()->constructor(object);

	tgui_viewport_t *viewport = TGUI_VIEWPORT_CAST(object);
	viewport->have_hscroll = 1;
	viewport->have_vscroll = 1;

	return 0;
}

static void tgui_viewport_class_init(tgui_viewport_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_viewport_calculate_sizes;
	widget_class->allocate_space  = tgui_viewport_allocate_space;
	widget_class->remove_child = tgui_viewport_remove_child;
	widget_class->render       = tgui_viewport_render;
	widget_class->after_render = tgui_viewport_after_render;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_viewport_constructor;
}

tgui_viewport_t *tgui_viewport_new(void) {
	return tobject_new(tgui_viewport_get_type());
}

void tgui_viewport_set_child(tgui_viewport_t *viewport, tgui_widget_t *child) {
	tgui_widget_destroy(viewport->child);
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(viewport));
	viewport->child = child;
}

void tgui_viewport_set_scroll_x(tgui_viewport_t *viewport, long scroll_x) {
	viewport->scroll_x = scroll_x;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(viewport));
}

void tgui_viewport_set_scroll_y(tgui_viewport_t *viewport, long scroll_y) {
	viewport->scroll_y = scroll_y;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(viewport));
}

void tgui_viewport_set_have_hscroll(tgui_viewport_t *viewport, char have_hscroll) {
	viewport->have_hscroll = have_hscroll;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(viewport));
}

void tgui_viewport_set_have_vscroll(tgui_viewport_t *viewport, char have_vscroll) {
	viewport->have_vscroll = have_vscroll;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(viewport));
}

tgui_widget_t *tgui_viewport_get_child(tgui_viewport_t *viewport) {
	return viewport->child;
}

long tgui_viewport_get_scroll_x(tgui_viewport_t *viewport) {
	return viewport->scroll_x;
}

long tgui_viewport_get_scroll_y(tgui_viewport_t *viewport) {
	return viewport->scroll_y;
}

char tgui_viewport_get_have_hscroll(tgui_viewport_t *viewport) {
	return viewport->have_hscroll;
}

char tgui_viewport_get_have_vscroll(tgui_viewport_t *viewport) {
	return viewport->have_vscroll;
}

long tgui_viewport_get_content_min_width(tgui_viewport_t *viewport) {
	if (viewport->child) {
		tgui_widget_calculate_sizes(viewport->child);
		return viewport->child->min_width;
	} else {
		// avoid the viewport collapsing
		return 10;
	}
}

long tgui_viewport_get_content_min_height(tgui_viewport_t *viewport) {
	if (viewport->child) {
		tgui_widget_calculate_sizes(viewport->child);
		return viewport->child->min_height;
	} else {
		// avoid the viewport collapsing
		return 10;
	}
}

long tgui_viewport_get_content_pref_width(tgui_viewport_t *viewport) {
	if (viewport->child) {
		tgui_widget_calculate_sizes(viewport->child);
		return viewport->child->pref_width;
	} else {
		// avoid the viewport collapsing
		return 10;
	}
}

long tgui_viewport_get_content_pref_height(tgui_viewport_t *viewport) {
	if (viewport->child) {
		tgui_widget_calculate_sizes(viewport->child);
		return viewport->child->pref_height;
	} else {
		// avoid the viewport collapsing
		return 10;
	}
}
