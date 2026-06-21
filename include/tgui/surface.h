#ifndef TGUI_SURFACE_H
#define TGUI_SURFACE_H

#include "widget.h"
#include "rect.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_surface, TGUI_SURFACE, tgui_widget)

struct tgui_surface {
	tgui_widget_t widget;
	tgui_list_node_t node;
	void *private;
	long scaling;
	tgui_widget_t *focus;
	tgui_widget_t *child;
	tgui_widget_t *hover;
	tgui_widget_t *clicked;
	tgui_rect_t inval;
	tgui_rect_t clip;
	long width;
	long height;
	long old_flags;
	int mouse_pressed;
};


tgui_surface_t *tgui_surface_new(long width, long height, tgui_surface_t *parent);
void tgui_surface_set_child(tgui_surface_t *surface, tgui_widget_t *child);
tgui_widget_t *tgui_surface_get_child(tgui_surface_t *surface);
int tgui_surface_resize(tgui_surface_t *surface, long width, long height);
void tgui_surface_render(tgui_surface_t *surface);
tgui_list_t *tgui_get_surfaces(void);
void tgui_surface_set_scaling(tgui_surface_t *surface, long scaling);
long tgui_surface_get_scaling(tgui_surface_t *surface);
void tgui_surface_set_position(tgui_surface_t *surface, long x, long y);
void tgui_surface_set_focus(tgui_surface_t *surface, tgui_widget_t *widget);
tgui_widget_t *tgui_surface_get_focus(tgui_surface_t *surface);
void tgui_surface_invalidate(tgui_surface_t *surface, tgui_rect_t *rect);
void tgui_surface_register(tgui_surface_t *surface);
void tgui_surface_unregister(tgui_surface_t *surface);

static inline tgui_surface_t *tgui_widget_get_surface(tgui_widget_t *widget) {
	while (widget) {
		if (tgui_widget_is_type(widget, tgui_surface_get_type())) {
			return TGUI_SURFACE_CAST(widget);
		}
		widget = widget->parent;
	}
	return NULL;
}

#endif
