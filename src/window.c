#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <widget.h>
#include <window.h>
#include <platform.h>

static tgui_list_t windows;

TOBJECT_DEFINE_CLASS(tgui_window, TGUI_WINDOW, tgui_surface_get_type())

static void tgui_window_calculate_sizes(tgui_widget_t *widget) {
	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	tgui_widget_t *bar = TGUI_WIDGET_CAST(window->title_bar);
	tgui_widget_t *child = tgui_window_get_child(window);
	if (!tgui_widget_is_hidden(bar)) {
		tgui_widget_calculate_sizes(bar);
		widget->min_width  = bar->min_width;
		widget->min_height = bar->min_height;
		widget->pref_width  = bar->pref_width;
		widget->pref_height = bar->pref_height;
	}
	if (child) {
		tgui_widget_calculate_sizes(child);
		if (widget->min_width < child->min_width) {
			widget->min_width = child->min_width;
		}
		widget->min_height += child->min_height;
		if (widget->pref_width < child->pref_width) {
			widget->pref_width = child->pref_width;
		}
		widget->pref_height += child->pref_height;
	}
}

static void tgui_window_allocate_space(tgui_widget_t *widget) {
	tgui_window_t *window = TGUI_WINDOW_CAST(widget);
	tgui_widget_t *bar = TGUI_WIDGET_CAST(window->title_bar);
	tgui_widget_t *child = tgui_window_get_child(window);
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);
	long bar_height = bar->min_height;
	if (tgui_widget_is_hidden(bar)) {
		tgui_widget_allocate_space(child, x, y, width, height);
	} else {
		tgui_widget_allocate_space(bar, x, y, width, bar_height);
		tgui_widget_allocate_space(child, x, y + bar_height, width, height - bar_height);
	}
}

static int tgui_window_constructor(void *object) {
	// skip the surface constructor
	tgui_widget_get_type()->class->constructor(object);
	
	tgui_window_t *window = TGUI_WINDOW_CAST(object);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(window), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(window), TGUI_TRUE);
	window->surface.scaling = 1;
	window->title_bar = tgui_title_bar_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(window->title_bar), TGUI_TRUE);
	tgui_widget_set_parent(TGUI_WIDGET_CAST(window->title_bar), TGUI_WIDGET_CAST(window));
	tgui_keyboardnav_set_widget(&TGUI_SURFACE_CAST(window)->keyboardnav, TGUI_WIDGET_CAST(window));
	return 0;
}

static int tgui_window_destructor(void *object) {
	tgui_window_t *window = TGUI_WINDOW_CAST(object);
	tgui_list_remove(&windows, &window->node);
	tgui_surface_unregister(TGUI_SURFACE_CAST(window));
	tgui_platform_close_window(window);
	free(window->title);
	return 0;
}

static void tgui_window_class_init(tgui_window_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_window_calculate_sizes;
	widget_class->allocate_space  = tgui_window_allocate_space;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_window_constructor;
	tobject_class->destructor  = tgui_window_destructor;
}

tgui_window_t *tgui_window_new_with_parent(const char *title, long width, long height, tgui_window_t *parent) {
	tgui_window_t *window = tobject_new(tgui_window_get_type());
	if (!window) return NULL;

	// TODO : move this to constructor
	window->surface.width  = width;
	window->surface.height = height;
	window->surface.height = height;
	window->title = strdup(title ? title : "tgui window");
	tgui_title_bar_set_title(window->title_bar, window->title);
	tgui_platform_create_window(window, parent);
	tgui_list_append(&windows, &window->node);
	tgui_surface_register(&window->surface);

	tgui_rect_t rect;
	tgui_rect_init(&rect, 0, 0, width, height);
	tgui_surface_invalidate(&window->surface, &rect);
	return window;
}

tgui_window_t *tgui_window_new(const char *title, long width, long height) {
	return tgui_window_new_with_parent(title, width, height, NULL);
}

void tgui_window_set_child(tgui_window_t *window, tgui_widget_t *child) {
	tgui_surface_set_child(&window->surface, child);
}

tgui_widget_t *tgui_window_get_child(tgui_window_t *window) {
	return tgui_surface_get_child(&window->surface);
}

int tgui_window_resize(tgui_window_t *window, long width, long height) {
	return tgui_surface_resize(&window->surface, width, height);
}

tgui_surface_t *tgui_window_get_surface(tgui_window_t *window) {
	return &window->surface;
}

tgui_list_t *tgui_get_windows(void) {
	return &windows;
}

void tgui_window_set_scaling(tgui_window_t *window, long scaling) {
	tgui_surface_set_scaling(&window->surface, scaling);
}

long tgui_window_get_scaling(tgui_window_t *window) {
	return tgui_surface_get_scaling(&window->surface);
}

void tgui_window_set_focus(tgui_window_t *window, tgui_widget_t *widget) {
	tgui_surface_set_focus(&window->surface, widget);
}

tgui_widget_t *tgui_window_get_focus(tgui_window_t *window) {
	return tgui_surface_get_focus(&window->surface);
}

void tgui_window_set_title_bar(tgui_window_t *window, int enabled) {
	tgui_widget_set_visible(TGUI_WIDGET_CAST(window->title_bar), enabled);
}
