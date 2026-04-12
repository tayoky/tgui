#include <menuitem.h>

static int tgui_menu_item_click(tgui_event_t *event) {
	tgui_menu_item_t *menu_item = TGUI_MENU_ITEM_CAST(event->widget);
	tgui_surface_t *surface = tgui_widget_get_surface(event->widget);
	tgui_action_trigger(menu_item->action, surface, event->widget, NULL);
	return TGUI_EVENT_HANDLED;
}

static tgui_widget_class_t menu_item_class = {
	.size = sizeof(tgui_menu_item_t),
	.name = "menu item",
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space  = tgui_container_single_allocate_space,
};

tgui_menu_item_t *tgui_menu_item_new(const char *action_name) {
	tgui_action_t *action = tgui_action_get_from_name(action_name);
	if (!action) return NULL;

	tgui_widget_t *widget = tgui_widget_new(&menu_item_class);
	if (!widget) return NULL;

	tgui_menu_item_t *menu_item = TGUI_MENU_ITEM_CAST(widget);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_menu_item_click, NULL);
	menu_item->action = action;
	menu_item->label = tgui_label_new(action->name);
	tgui_widget_set_parent(TGUI_WIDGET_CAST(menu_item->label), TGUI_WIDGET_CAST(menu_item));
	return menu_item;
}

tgui_action_t *tgui_menu_item_get_action(tgui_menu_item_t *menu_item) {
	return menu_item->action;
}
