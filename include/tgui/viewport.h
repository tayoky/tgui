#ifndef TGUI_VIEWPORT_H
#define TGUI_VIEWPORT_H

#include <widget.h>

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_viewport, TGUI_VIEWPORT, tgui_widget)

struct tgui_viewport {
	tgui_widget_t widget;
	tgui_widget_t *child;
	long scroll_x;
	long scroll_y;
	char have_hscroll;
	char have_vscroll;
	long old_clip_x;
	long old_clip_y;
	long old_clip_width;
	long old_clip_height;
};

tgui_viewport_t *tgui_viewport_new(void);
void tgui_viewport_set_child(tgui_viewport_t *viewport, tgui_widget_t *child);
void tgui_viewport_set_scroll_x(tgui_viewport_t *viewport, long scroll_x);
void tgui_viewport_set_scroll_y(tgui_viewport_t *viewport, long scroll_y);
void tgui_viewport_set_have_hscroll(tgui_viewport_t *viewport, char have_hscroll);
void tgui_viewport_set_have_vscroll(tgui_viewport_t *viewport, char have_vscroll);
tgui_widget_t *tgui_viewport_get_child(tgui_viewport_t *viewport);
long tgui_viewport_get_scroll_x(tgui_viewport_t *viewport);
long tgui_viewport_get_scroll_y(tgui_viewport_t *viewport);
char tgui_viewport_get_have_hscroll(tgui_viewport_t *viewport);
char tgui_viewport_get_have_vscroll(tgui_viewport_t *viewport);
long tgui_viewport_get_content_min_width(tgui_viewport_t *viewport);
long tgui_viewport_get_content_min_height(tgui_viewport_t *viewport);
long tgui_viewport_get_content_pref_width(tgui_viewport_t *viewport);
long tgui_viewport_get_content_pref_height(tgui_viewport_t *viewport);

#endif
