#ifndef TGUI_TEXT_VIEW_H
#define TGUI_TEXT_VIEW_H

#include "textbuffer.h"
#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_text_view, TGUI_TEXT_VIEW, tgui_widget)

struct tgui_text_view {
	tgui_widget_t widget;
	tgui_text_buffer_t *buffer;
};

tgui_text_view_t *tgui_text_view_new(void);
void tgui_text_view_set_buffer(tgui_text_view_t *text_view, tgui_text_buffer_t *buffer);
tgui_text_buffer_t *tgui_text_view_get_buffer(tgui_text_view_t *text_view);

#endif
