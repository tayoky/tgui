#ifndef TGUI_SUBMENU_BUTTON_H
#define TGUI_SUBMENU_BUTTON_H

#include "widget.h"
#include "popover.h"
#include "popoverbutton.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_submenu_button, TGUI_SUBMENU_BUTTON, tgui_popover_button)

struct tgui_submenu_button {
	tgui_popover_button_t popover_button;
};

tgui_popover_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name);

#endif
