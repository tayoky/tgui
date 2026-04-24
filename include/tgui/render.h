#ifndef TGUI_RENDER_H
#define TGUI_RENDER_H

#include "widget.h"
#include "surface.h"
#include "rect.h"

void tgui_render_rect(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height);
void tgui_render_text(tgui_widget_t *widget, long x, long y, const char *text);
void tgui_render_set_clip(tgui_surface_t *surface, tgui_rect_t *rect);
void tgui_render_get_clip(tgui_surface_t *surface, tgui_rect_t *rect);
void tgui_render_widget_base(tgui_widget_t *widget);

#endif
