#ifndef TGUI_BUTTON_H
#define TGUI_BUTTON_H

#include "widget.h"

typedef struct tgui_button {
	tgui_widget_t widget;
} tgui_button_t;

#define TGUI_BUTTON_CAST(w) TGUI_CONTAINER_OF(w, tgui_button_t, widget)

tgui_button_t *tgui_button_new(void);
void tgui_button_set_text(tgui_button_t *button, const char *text);
void tgui_button_set_icon(tgui_button_t *button, const char *icon);
void tgui_button_set_child(tgui_button_t *button, tgui_widget_t *child);
const char *tgui_button_get_text(tgui_button_t *button);
const char *tgui_button_get_icon(tgui_button_t *button);
tgui_widget_t *tgui_button_get_child(tgui_button_t *button);

#endif
