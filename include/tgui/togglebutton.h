#ifndef TGUI_TOGGLE_BUTTON_H
#define TGUI_TOGGLE_BUTTON_H

#include "button.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_toggle_button, TGUI_TOGGLE_BUTTON, tgui_button)

struct tgui_toggle_button {
	tgui_button_t button;
	int active;
};

tgui_toggle_button_t *tgui_toggle_button_new(void);
void tgui_toggle_button_set_active(tgui_toggle_button_t *toggle_button, int active);
int tgui_toggle_button_get_active(tgui_toggle_button_t *toggle_button);

#endif
