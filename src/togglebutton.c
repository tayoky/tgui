#include <togglebutton.h>

TOBJECT_DEFINE_CLASS(tgui_toggle_button, TGUI_TOGGLE_BUTTON, tgui_button_get_type())

static void tgui_toggle_button_click(tobject_t *tobject) {
	tgui_toggle_button_t *toggle_button = TGUI_TOGGLE_BUTTON_CAST(tobject);
	// we toggle, that the whole point
	tgui_toggle_button_set_active(toggle_button, !tgui_toggle_button_get_active(toggle_button));
}

static int tgui_toggle_button_constructor(void *object) {
	// we skip the tgui_button constructor
	// to avoid the button's click to mess up with toggle logic
	tgui_button_get_type()->parent_type->class->constructor(object);

	tgui_widget_connect_signal(TGUI_WIDGET_CAST(object), "click", TCALLBACK_CAST(tgui_toggle_button_click), NULL);

	// buttons are focusable by default
	tgui_widget_set_focusable(object, TGUI_TRUE);

	return 0;
}

static void tgui_toggle_button_class_init(tgui_toggle_button_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_toggle_button_constructor;
}

tgui_toggle_button_t *tgui_toggle_button_new(void) {
	return tobject_new(tgui_toggle_button_get_type());
}

void tgui_toggle_button_set_active(tgui_toggle_button_t *toggle_button, int active) {
	if (toggle_button->active == active) return;
	toggle_button->active = active;
	tgui_widget_set_state(TGUI_WIDGET_CAST(toggle_button), TGUI_STATE_PRESSED, active);
	tgui_widget_send_signal(TGUI_WIDGET_CAST(toggle_button), "toggled", &active);
	tgui_widget_send_signal(TGUI_WIDGET_CAST(toggle_button), "changed", &active);
}

int tgui_toggle_button_get_active(tgui_toggle_button_t *toggle_button) {
	return toggle_button->active;
}
