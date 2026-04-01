#ifndef TGUI_WINDOW_H
#define TGUI_WINDOW_H

#include "widget.h"

typedef struct tgui_window {
	tgui_widget_t widget;
	tgui_list_node_t node;
	void *private;
	long scaling;
	tgui_widget_t *focus;
	char *title;
	long inval_start_x;
	long inval_start_y; 
	long inval_end_x;
	long inval_end_y;
} tgui_window_t;

#define TGUI_WINDOW_CAST(w) TGUI_CONTAINER_OF(w, tgui_window_t, widget)

tgui_window_t *tgui_window_new(const char *title, long width, long height);
void tgui_window_set_child(tgui_window_t *window, tgui_widget_t *child);
int tgui_window_resize(tgui_window_t *window, long width, long height);
void tgui_window_render(tgui_window_t *window);
tgui_list_t *tgui_get_windows(void);
void tgui_window_set_scaling(tgui_window_t *window, long scaling);
long tgui_window_get_scaling(tgui_window_t *window);
void tgui_window_set_focus(tgui_window_t *window, tgui_widget_t *widget);
tgui_widget_t *tgui_window_get_focus(tgui_window_t *window);
void tgui_window_invalidate(tgui_window_t *window, long x, long y, long width, long height);

static inline tgui_window_t *tgui_widget_get_window(tgui_widget_t *widget) {
	while (widget) {
		if (tgui_widget_is_class(widget, "window")) {
			return TGUI_WINDOW_CAST(widget);
		}
		widget = widget->parent;
	}
	return NULL;
}

#endif
