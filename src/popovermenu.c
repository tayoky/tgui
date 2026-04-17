#include <popovermenu.h>
#include <menuitem.h>

TOBJECT_DEFINE_CLASS(tgui_popover_menu, TGUI_POPOVER_MENU, tgui_popover_get_type())

static void tgui_popover_menu_class_init(tgui_popover_menu_class_t *class) {
	(void)class;
}

tgui_popover_menu_t *tgui_popover_menu_new(void) {
	return tobject_new(tgui_popover_menu_get_type());
}

void tgui_popover_menu_append(tgui_popover_menu_t *menu, tgui_widget_t *child) {
	tgui_box_append_widget(menu->box, child);
}

void tgui_popover_menu_append_action(tgui_popover_menu_t *menu, const char *action) {
	tgui_menu_item_t *menu_item = tgui_menu_item_new(action);
	tgui_popover_menu_append(menu, TGUI_WIDGET_CAST(menu_item));
}

void tgui_popover_menu_append_submenu(tgui_popover_menu_t *menu, tgui_popover_menu_t *submenu);
