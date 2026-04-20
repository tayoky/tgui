#ifndef TGUI_MENU_ITEM_H
#define TGUI_MENU_ITEM_H

#include "widget.h"
#include "action.h"
#include "label.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_menu_item, TGUI_MENU_ITEM, tgui_widget)

struct tgui_menu_item {
	tgui_widget_t widget;
	tgui_action_t *action;
	tgui_label_t *label;
};

/**
 * @brief create a new menu item
 * @param action the action id to associate the new menu item with
 * @return a new menu item
 */
tgui_menu_item_t *tgui_menu_item_new(const char *action);

/**
 * @brief get the action associed with a menu item
 * @param menu_item the menu item to query the action of
 * @return the associed action
 */
tgui_action_t *tgui_menu_item_get_action(tgui_menu_item_t *menu_item);

#endif
