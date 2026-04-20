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

#define TGUI_DIRECTION_LEFT    0
#define TGUI_DIRECTION_RIGHT   1
#define TGUI_DIRECTION_TOP     2
#define TGUI_DIRECTION_BOTTOM  3

tgui_submenu_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name);
void tgui_submenu_button_set_popover(tgui_submenu_button_t *submenu_button, tgui_popover_t *popover);
tgui_popover_t *tgui_submenu_button_get_popover(tgui_submenu_button_t *submenu_button);
void tgui_submenu_button_set_direction(tgui_submenu_button_t *submenu_button, char direction);

#endif
