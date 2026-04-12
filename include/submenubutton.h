#ifndef TGUI_SUBMENU_BUTTON_H
#define TGUI_SUBMENU_BUTTON_H

#include "widget.h"
#include "popover.h"
#include "button.h"

typedef struct tgui_submenu_button {
	tgui_button_t button;
	tgui_popover_t *popover;
	char direction;
} tgui_submenu_button_t;

#define TGUI_SUBMENU_BUTTON_CAST(w) TGUI_CONTAINER_OF(w, tgui_submenu_button_t, button.widget)

tgui_submenu_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name);
void tgui_submenu_button_set_popover(tgui_submenu_button_t *submenu_button, tgui_popover_t *popover);
tgui_popover_t *tgui_submenu_button_get_popover(tgui_submenu_button_t *submenu_button);

#endif
