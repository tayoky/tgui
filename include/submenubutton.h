#ifndef TGUI_SUBMENU_BUTTON_H
#define TGUI_SUBMENU_BUTTON_H

#include "widget.h"
#include "popover.h"
#include "button.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_submenu_button, TGUI_SUBMENU_BUTTON, tgui_button)

struct tgui_submenu_button {
	tgui_button_t button;
	tgui_popover_t *popover;
	char direction;
};

tgui_submenu_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name);
void tgui_submenu_button_set_popover(tgui_submenu_button_t *submenu_button, tgui_popover_t *popover);
tgui_popover_t *tgui_submenu_button_get_popover(tgui_submenu_button_t *submenu_button);

#endif
