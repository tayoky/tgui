#ifndef TGUI_POPOVER_H
#define TGUI_POPOVER_H

#include "widget.h"
#include "surface.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_popover, TGUI_POPOVER, tobject)

struct tgui_popover {
	tobject_t tobject;
	tgui_surface_t *parent;
	tgui_surface_t *surface;
	tgui_widget_t *child;
	long x;
	long y;
	int popped;
};

tgui_popover_t *tgui_popover_new(tgui_surface_t *parent);
static inline void tgui_popover_release(tgui_popover_t *popover) {
	tobject_free(TOBJECT_CAST(popover));
}
void tgui_popover_set_child(tgui_popover_t *popover, tgui_widget_t *child);
void tgui_popover_set_position(tgui_popover_t *popover, long x, long y);
tgui_widget_t *tgui_popover_get_child(tgui_popover_t *popover);
void tgui_popover_popdown(tgui_popover_t *popover);
void tgui_popover_popup(tgui_popover_t *popover);
long tgui_popover_get_width(tgui_popover_t *popover);
long tgui_popover_get_height(tgui_popover_t *popover);

#endif
