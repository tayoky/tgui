#include <popoverbutton.h>

TOBJECT_DEFINE_CLASS(tgui_popover_button, TGUI_POPOVER_BUTTON, tgui_button_get_type())

static void tgui_popover_button_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	TGUI_WIDGET_CLASS_CAST(tgui_popover_button_get_parent_class())->remove_child(widget, child);

	tgui_popover_button_t *popover_button = TGUI_POPOVER_BUTTON_CAST(widget);
	if (TGUI_WIDGET_CAST(popover_button->popover) == child) {
		popover_button->popover = NULL;
	}
}

static int tgui_popover_button_click(tgui_event_t *event) {
	tgui_popover_button_t *popover_button = TGUI_POPOVER_BUTTON_CAST(event->widget);
	long x = tgui_widget_get_frame_x(TGUI_WIDGET_CAST(popover_button));
	long y = tgui_widget_get_frame_y(TGUI_WIDGET_CAST(popover_button));
	long height = tgui_widget_get_frame_height(TGUI_WIDGET_CAST(popover_button));
	long width = tgui_widget_get_frame_height(TGUI_WIDGET_CAST(popover_button));
	long popover_width = tgui_popover_get_width(popover_button->popover);
	long popover_height = tgui_popover_get_width(popover_button->popover);
	switch (popover_button->direction) {
	case TGUI_DIRECTION_LEFT:
		x -= popover_width;
		break;
	case TGUI_DIRECTION_RIGHT:
		x += width;
		break;
	case TGUI_DIRECTION_TOP:
		y -= popover_height;
		break;
	case TGUI_DIRECTION_BOTTOM:
		y += height;
		break;
	}
	
	tgui_popover_set_position(popover_button->popover, x, y);
	tgui_popover_popup(popover_button->popover);
	return TGUI_EVENT_HANDLED;
}

static int tgui_popover_button_constructor(void *object) {
	tgui_popover_button_get_parent_class()->constructor(object);

	tgui_popover_button_t *popover_button = TGUI_POPOVER_BUTTON_CAST(object);
	popover_button->direction = TGUI_DIRECTION_BOTTOM;

	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_popover_button_click, NULL);
	return 0;
}

static void tgui_popover_button_class_init(tgui_popover_button_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->remove_child = tgui_popover_button_remove_child;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_popover_button_constructor;
}

tgui_popover_button_t *tgui_popover_button_new(tgui_popover_t *popover, const char *name) {
	tgui_popover_button_t *popover_button = tobject_new(tgui_popover_button_get_type());
	if (!popover_button) return NULL;

	tgui_popover_button_set_popover(popover_button, popover);
	tgui_button_set_text(TGUI_BUTTON_CAST(popover_button), name);
	return popover_button;
}

void tgui_popover_button_set_popover(tgui_popover_button_t *popover_button, tgui_popover_t *popover) {
	tgui_widget_destroy(TGUI_WIDGET_CAST(popover_button->popover));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(popover), TGUI_WIDGET_CAST(popover_button));
	popover_button->popover = popover;
}

tgui_popover_t *tgui_popover_button_get_popover(tgui_popover_button_t *popover_button) {
	return popover_button->popover;
}

void tgui_popover_button_set_direction(tgui_popover_button_t *popover_button, char direction) {
	popover_button->direction = direction;
}
