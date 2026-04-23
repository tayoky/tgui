#ifndef TGUI_SCROLLED_WINDOW_H
#define TGUI_SCROLLED_WINDOW_H

#include "widget.h"
#include "scrollbar.h"
#include "viewport.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_scrolled_window, TGUI_SCROLLED_WINDOW, tgui_widget)

struct tgui_scrolled_window {
	tgui_widget_t widget;
	tgui_scrollbar_t *hbar;
	tgui_scrollbar_t *vbar;
	tgui_viewport_t *viewport;
	int hpolicy;
	int vpolicy;
};

#define TGUI_SCROLLED_WINDOW_POLICY_AUTO   0
#define TGUI_SCROLLED_WINDOW_POLICY_NEVER  1
#define TGUI_SCROLLED_WINDOW_POLICY_ALWAYS 2

tgui_scrolled_window_t *tgui_scrolled_window_new(void);
void tgui_scrolled_window_set_child(tgui_scrolled_window_t *scrolled_window, tgui_widget_t *child);
void tgui_scrolled_window_set_hpolicy(tgui_scrolled_window_t *scrolled_window, int hpolicy);
void tgui_scrolled_window_set_vpolicy(tgui_scrolled_window_t *scrolled_window, int vpolicy);
tgui_widget_t *tgui_scrolled_window_get_child(tgui_scrolled_window_t *scrolled_window);
int tgui_scrolled_window_get_hpolicy(tgui_scrolled_window_t *scrolled_window);
int tgui_scrolled_window_get_vpolicy(tgui_scrolled_window_t *scrolled_window);

#endif
