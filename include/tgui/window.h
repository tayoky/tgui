#ifndef TGUI_WINDOW_H
#define TGUI_WINDOW_H

#include "widget.h"
#include "surface.h"
#include "titlebar.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_window, TGUI_WINDOW, tgui_surface)

struct tgui_window {
	tgui_surface_t surface;
	tgui_list_node_t node;
	tgui_title_bar_t *title_bar;
	char *title;
};

tgui_window_t *tgui_window_new(const char *title, long width, long height);
void tgui_window_set_child(tgui_window_t *window, tgui_widget_t *child);
tgui_widget_t *tgui_window_get_child(tgui_window_t *window);
int tgui_window_resize(tgui_window_t *window, long width, long height);
tgui_list_t *tgui_get_windows(void);
void tgui_window_set_scaling(tgui_window_t *window, long scaling);
long tgui_window_get_scaling(tgui_window_t *window);
void tgui_window_set_focus(tgui_window_t *window, tgui_widget_t *widget);
tgui_widget_t *tgui_window_get_focus(tgui_window_t *window);
void tgui_window_set_title_bar(tgui_window_t *window, int enabled);
tgui_surface_t *tgui_window_get_surface(tgui_window_t *window);

static inline tgui_window_t *tgui_widget_get_window(tgui_widget_t *widget) {
	while (widget) {
		if (tgui_widget_is_type(widget, tgui_window_get_type())) {
			return TGUI_WINDOW_CAST(widget);
		}
		widget = widget->parent;
	}
	return NULL;
}

#endif
