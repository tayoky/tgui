#include <submenubutton.h>

static int tgui_submenu_button_click(tgui_event_t *event) {
	tgui_submenu_button_t *submenu_button = TGUI_SUBMENU_BUTTON_CAST(event->widget);
	// TODO : use direction
	long x = tgui_widget_get_frame_x(TGUI_WIDGET_CAST(submenu_button));
	long y = tgui_widget_get_frame_y(TGUI_WIDGET_CAST(submenu_button));
	long height = tgui_widget_get_frame_height(TGUI_WIDGET_CAST(submenu_button));
	tgui_popover_set_position(submenu_button->popover, x, y + height);
	tgui_popover_popup(submenu_button->popover);
	return TGUI_EVENT_HANDLED;
}

static tgui_widget_class_t submenu_button_class = {
	.size = sizeof(tgui_submenu_button_t),
	.name = "submenu button",
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space  = tgui_container_single_allocate_space,
};

tgui_submenu_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name) {
	tgui_widget_t *widget = tgui_widget_new(&submenu_button_class);

	tgui_submenu_button_t *submenu_button = TGUI_SUBMENU_BUTTON_CAST(widget);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_submenu_button_click, NULL);
	submenu_button->popover = popover;

	tgui_button_set_text(&submenu_button->button, name);
	return submenu_button;
}

void tgui_submenu_button_set_popover(tgui_submenu_button_t *submenu_button, tgui_popover_t *popover) {
	submenu_button->popover = popover;
}

tgui_popover_t *tgui_submenu_button_get_popover(tgui_submenu_button_t *submenu_button) {
	return submenu_button->popover;
}
