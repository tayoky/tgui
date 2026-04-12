#ifndef TGUI_POPOVER_MENU_H
#define TGUI_POPOVER_MENU_H

#include "popover.h"
#include "action.h"
#include "widget.h"
#include "box.h"

typedef struct tgui_popover_menu {
	tgui_popover_t popover;
	tgui_box_t *box;
} tgui_popover_menu_t;

tgui_popover_menu_t *tgui_popover_menu_new(void);
void tgui_popover_menu_append(tgui_popover_menu_t *menu, tgui_widget_t *child);
void tgui_popover_menu_append_action(tgui_popover_menu_t *menu, const char *action);
void tgui_popover_menu_append_submenu(tgui_popover_menu_t *menu, tgui_popover_menu_t *submenu);

#endif
