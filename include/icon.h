#ifndef TGUI_ICON_H
#define TGUI_ICON_H

#include "widget.h"
#include "image.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_icon, TGUI_ICON, tgui_widget)

struct tgui_icon {
	tgui_widget_t widget;
	tgui_image_t *image;
};

tgui_icon_t *tgui_icon_new(const char *name);
void tgui_icon_set(tgui_icon_t *icon, const char *name);
void tgui_icon_set_image(tgui_icon_t *icon, tgui_image_t *image);
tgui_image_t *tgui_icon_get_image(tgui_icon_t *icon);

#endif
