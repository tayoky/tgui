#include <checkbox.h>

TOBJECT_DEFINE_CLASS(tgui_checkbox, TGUI_CHECKBOX, tgui_toggle_button_get_type())

static void tgui_checkbox_toggled(tobject_t *tobject) {
	tgui_checkbox_t *checkbox = TGUI_CHECKBOX_CAST(tobject);
	if (tgui_checkbox_get(checkbox)) {
		tgui_button_set_icon(TGUI_BUTTON_CAST(checkbox), "checkbox-checked");
	} else {
		tgui_button_set_icon(TGUI_BUTTON_CAST(checkbox), "checkbox-blank");
	}
}

static int tgui_checkbox_constructor(void *object) {
	tgui_checkbox_get_parent_class()->constructor(object);

	tgui_button_set_icon(TGUI_BUTTON_CAST(object), "checkbox-blank");
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(object), "toggled", TCALLBACK_CAST(tgui_checkbox_toggled), NULL);
	return 0;
}

static void tgui_checkbox_class_init(tgui_checkbox_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_checkbox_constructor;
}

tgui_checkbox_t *tgui_checkbox_new(void) {
	return tobject_new(tgui_checkbox_get_type());
}
