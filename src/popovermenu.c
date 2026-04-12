#include <popovermenu.h>
#include <menuitem.h>

static tgui_widget_class_t popover_menu_class = {
	.size = sizeof(tgui_popover_menu_t),
	.name = "popover menu",
};

tgui_popover_menu_t *tgui_popover_menu_new(void) {
}

void tgui_popover_menu_append(tgui_popover_menu_t *menu, tgui_widget_t *child) {
	tgui_box_append_widget(menu->box, child);
}

void tgui_popover_menu_append_action(tgui_popover_menu_t *menu, const char *action) {
	tgui_menu_item_t *menu_item = tgui_menu_item_new(action);
	tgui_popover_menu_append(menu, TGUI_WIDGET_CAST(menu_item));
}

void tgui_popover_menu_append_submenu(tgui_popover_menu_t *menu, tgui_popover_menu_t *submenu);
