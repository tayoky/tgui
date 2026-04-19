#include <submenubutton.h>

TOBJECT_DEFINE_CLASS(tgui_submenu_button, TGUI_SUBMENU_BUTTON, tgui_button_get_type())

static int tgui_submenu_button_click(tgui_event_t *event) {
	tgui_submenu_button_t *submenu_button = TGUI_SUBMENU_BUTTON_CAST(event->widget);
	long x = tgui_widget_get_frame_x(TGUI_WIDGET_CAST(submenu_button));
	long y = tgui_widget_get_frame_y(TGUI_WIDGET_CAST(submenu_button));
	long height = tgui_widget_get_frame_height(TGUI_WIDGET_CAST(submenu_button));
	long width = tgui_widget_get_frame_height(TGUI_WIDGET_CAST(submenu_button));

	switch (submenu_button->direction) {
	case TGUI_DIRECTION_LEFT:
		// TODO
		break;
	case TGUI_DIRECTION_RIGHT:
		x += width;
		break;
	case TGUI_DIRECTION_TOP:
		// TODO
		break;
	case TGUI_DIRECTION_BOTTOM:
		y += height;
		break;
	}
	
	tgui_popover_set_position(submenu_button->popover, x, y);
	tgui_popover_popup(submenu_button->popover);
	return TGUI_EVENT_HANDLED;
}

static int tgui_submenu_button_constructor(void *object) {
	tgui_submenu_button_get_parent_class()->constructor(object);

	tgui_submenu_button_t *submenu_button = TGUI_SUBMENU_BUTTON_CAST(object);
	submenu_button->direction = TGUI_DIRECTION_BOTTOM;

	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_submenu_button_click, NULL);
	return 0;
}

static void tgui_submenu_button_class_init(tgui_submenu_button_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_submenu_button_constructor;
}

tgui_submenu_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name) {
	tgui_submenu_button_t *submenu_button = tobject_new(tgui_submenu_button_get_type());
	if (!submenu_button) return NULL;

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

void tgui_submenu_button_set_direction(tgui_submenu_button_t *submenu_button, char direction) {
	submenu_button->direction = direction;
}
