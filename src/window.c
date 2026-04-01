#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <widget.h>
#include <window.h>
#include <platform.h>

static tgui_list_t windows;

static void tgui_window_free(tgui_widget_t *widget) {
	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	tgui_list_remove(&windows, &window->node);
	tgui_platform_close_window(window);
	free(window->title);
}

static tgui_widget_class_t window_class = {
	.name = "window",
	.size = sizeof(tgui_window_t),
	.free = tgui_window_free,
	.calculate_sizes = tgui_container_single_calculate_sizes,
};

static void tgui_window_reset_dirty(tgui_window_t *window) {
	window->inval_start_x = LONG_MAX;
	window->inval_start_y = LONG_MAX;
	window->inval_end_x = 0;
	window->inval_end_y = 0;
}

tgui_window_t *tgui_window_new(const char *title, long width, long height) {
	tgui_widget_t *widget = tgui_widget_new(&window_class);
	if (!widget) return NULL;


	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	window->widget.width  = width;
	window->widget.height = height;
	window->scaling = 1;
	window->title = strdup(title ? title : "tgui window");
	tgui_list_append(&windows, &window->node);

	tgui_platform_create_window(window);
	tgui_window_invalidate(window, 0, 0, width, height);
	return window;
}

void tgui_window_set_child(tgui_window_t *window, tgui_widget_t *child) {
	// if we aready have a child destroy it
	if (window->widget.children.first) {
		tgui_widget_destroy(TGUI_WIDGET_FROM_NODE(window->widget.children.first));
	}
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(window));
}

static void tgui_window_update_sizes(tgui_window_t *window) {
	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(window));
	if (window->widget.children.first) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(window->widget.children.first);
		tgui_widget_allocate_space(child, 0, 0, window->widget.width / window->scaling, window->widget.height / window->scaling);
	}
}

int tgui_window_resize(tgui_window_t *window, long width, long height) {
	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(window));
	if (width < window->widget.min_width || width < window->widget.min_height) {
		// won't do it
		// too small
		return -1;
	}
	window->widget.width = width;
	window->widget.height = height;

	tgui_window_update_sizes(window);
	return 0;
}

static int tgui_window_is_dirty(tgui_window_t *window) {
	return window->inval_start_x != LONG_MAX;
}

void tgui_window_render(tgui_window_t *window) {
	if (tgui_widget_is_dirty_size(TGUI_WIDGET_CAST(window))) {
		tgui_window_update_sizes(window);
	}
	if (tgui_window_is_dirty(window)) {
		printf("got dirty rect from %ld %ld to %ld %ld\n", window->inval_start_x, window->inval_start_y, window->inval_end_x, window->inval_end_y);
		tgui_platform_set_clip(window, window->inval_start_x, window->inval_start_y, 
		(window->inval_end_x - window->inval_start_x) * window->scaling, 
		(window->inval_end_y - window->inval_start_y) * window->scaling);
		if (window->widget.children.first) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(window->widget.children.first);
			tgui_widget_render(child);
		}
		tgui_platform_push_window(window);
	}
	tgui_window_reset_dirty(window);
}

tgui_list_t *tgui_get_windows(void) {
	return &windows;
}

void tgui_window_set_scaling(tgui_window_t *window, long scaling) {
	window->scaling = scaling;
	tgui_window_update_sizes(window);
}

long tgui_window_get_scaling(tgui_window_t *window) {
	return window->scaling;
}

void tgui_window_set_focus(tgui_window_t *window, tgui_widget_t *widget) {
	window->focus = widget;
}

tgui_widget_t *tgui_window_get_focus(tgui_window_t *window) {
	return window->focus;
}

void tgui_window_invalidate(tgui_window_t *window, long x, long y, long width, long height) {
	if (width == 0 || height == 0) return;
	long end_x = x + width;
	long end_y = y + height;
	if (x < window->inval_start_x) {
		window->inval_start_x = x;
	}
	if (y < window->inval_start_y) {
		window->inval_start_y = y;
	}
	if (end_x > window->inval_end_x) {
		window->inval_end_x = end_x;
	}
	if (end_y > window->inval_end_y) {
		window->inval_end_y = end_y;
	}
}
