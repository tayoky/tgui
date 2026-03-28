#ifndef TGUI_RENDER_H
#define TGUI_RENDER_H

#include "widget.h"

void tgui_render_rect(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height);
void tgui_render_text(tgui_widget_t *widget, long x, long y, const char *text);
void tgui_render_widget_base(tgui_widget_t *widget);

#endif
