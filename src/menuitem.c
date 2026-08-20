#include <menuitem.h>

TOBJECT_DEFINE_CLASS(tgui_menu_item, TGUI_MENU_ITEM, tgui_widget_get_type())

static void tgui_menu_item_click(tobject_t *tobject, tgui_event_click_t *event) {
	(void)event;
	tgui_menu_item_t *menu_item = TGUI_MENU_ITEM_CAST(tobject);
	tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(menu_item));
	tgui_action_trigger(menu_item->action, surface, TGUI_WIDGET_CAST(menu_item), NULL);
}

static int tgui_menu_item_constructor(void *object) {
	tgui_menu_item_get_parent_class()->constructor(object);
	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_connect_signal(widget, "click", TCALLBACK_CAST(tgui_menu_item_click), NULL);
	return 0;
}

static void tgui_menu_item_class_init(tgui_menu_item_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_menu_item_constructor;
}


tgui_menu_item_t *tgui_menu_item_new(const char *action_name) {
	tgui_action_t *action = tgui_action_get_from_name(action_name);
	if (!action) return NULL;

	tgui_menu_item_t *menu_item = tobject_new(tgui_menu_item_get_type());
	if (!menu_item) return NULL;
	menu_item->action = action;
	menu_item->label = tgui_label_new(action->name);
	tgui_widget_set_parent(TGUI_WIDGET_CAST(menu_item->label), TGUI_WIDGET_CAST(menu_item));
	return menu_item;
}

tgui_action_t *tgui_menu_item_get_action(tgui_menu_item_t *menu_item) {
	return menu_item->action;
}
