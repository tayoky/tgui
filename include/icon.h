#ifndef TGUI_ICON_H
#define TGUI_ICON_H

#include "widget.h"
#include "image.h"

typedef struct tgui_icon {
	tgui_widget_t widget;
	tgui_image_t *image;
} tgui_icon_t;

#define TGUI_ICON_CAST(w) TGUI_CONTAINER_OF(w, tgui_icon_t, widget)

tgui_icon_t *tgui_icon_new(const char *name);
void tgui_icon_set(tgui_icon_t *icon, const char *name);
void tgui_icon_set_image(tgui_icon_t *icon, tgui_image_t *image);
tgui_image_t *tgui_icon_get_image(tgui_icon_t *icon);

#endif
