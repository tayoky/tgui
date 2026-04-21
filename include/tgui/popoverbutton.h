#ifndef TGUI_POPOVER_BUTTON_H
#define TGUI_POPOVER_BUTTON_H

#include "widget.h"
#include "popover.h"
#include "button.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_popover_button, TGUI_POPOVER_BUTTON, tgui_button)

struct tgui_popover_button {
	tgui_button_t button;
	tgui_popover_t *popover;
	char direction;
};

#define TGUI_DIRECTION_LEFT    0
#define TGUI_DIRECTION_RIGHT   1
#define TGUI_DIRECTION_TOP     2
#define TGUI_DIRECTION_BOTTOM  3

tgui_popover_button_t *tgui_popover_button_new(tgui_popover_t *popover, const char *name);
void tgui_popover_button_set_popover(tgui_popover_button_t *popover_button, tgui_popover_t *popover);
tgui_popover_t *tgui_popover_button_get_popover(tgui_popover_button_t *popover_button);
void tgui_popover_button_set_direction(tgui_popover_button_t *popover_button, char direction);

#endif
