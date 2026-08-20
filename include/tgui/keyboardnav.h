#ifndef TGUI_KEYBOARDNAV_H
#define TGUI_KEYBOARDNAV_H

#include "widget.h"

typedef struct tgui_keyboardnav {
	tgui_widget_t *widget;
	size_t press_callback;
	size_t release_callback;
	size_t destroy_callback;
} tgui_keyboardnav_t;

void tgui_keyboardnav_set_widget(tgui_keyboardnav_t *keyboardnav, tgui_widget_t *widget);

#endif
