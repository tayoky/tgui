#include <widget.h>
#include <window.h>
#include <platform.h>

static tgui_list_t windows;

static void tgui_window_free(tgui_widget_t *widget) {
	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	tgui_list_remove(&windows, &window->node);
	tgui_platform_close_window(window);
}

static tgui_widget_class_t window_class = {
	.name = "window",
	.size = sizeof(tgui_window_t),
	.free = tgui_window_free,
	.calculate_sizes = tgui_container_single_calculate_sizes,
};

tgui_window_t *tgui_window_new(const char *title, long width, long height) {
	tgui_widget_t *widget = tgui_widget_new(&window_class);
	if (!widget) return NULL;


	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	window->widget.width  = width;
	window->widget.height = height;
	window->scaling = 1;
	tgui_list_append(&windows, &window->node);

	tgui_platform_create_window(window);
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

void tgui_window_render(tgui_window_t *window) {
	if (tgui_widget_is_dirty_size(TGUI_WIDGET_CAST(window))) {
		tgui_window_update_sizes(window);
	}
	if (tgui_widget_has_dirty_child(TGUI_WIDGET_CAST(window))) {
		if (window->widget.children.first) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(window->widget.children.first);
			tgui_widget_render(child);
		}
		tgui_platform_push_window(window);
	}
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
