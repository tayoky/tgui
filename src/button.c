#include <widget.h>
#include <button.h>
#include <label.h>
#include <icon.h>

TOBJECT_DEFINE_CLASS(tgui_button, TGUI_BUTTON, tgui_widget_get_type())

static void tgui_button_class_init(tgui_button_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_container_single_calculate_sizes;
	widget_class->allocate_space = tgui_container_single_allocate_space;
}

tgui_button_t *tgui_button_new(void) {
	return tobject_new(tgui_button_get_type());
}

void tgui_button_set_text(tgui_button_t *button, const char *text) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_label_get_type())) {
		// we already have a label
		tgui_label_set_text(TGUI_LABEL_CAST(child), text);
	} else {
		tgui_label_t *label = tgui_label_new(text);
		tgui_button_set_child(button, TGUI_WIDGET_CAST(label));
		tgui_widget_set_hexpand(TGUI_WIDGET_CAST(label), TGUI_TRUE);
		tgui_widget_set_vexpand(TGUI_WIDGET_CAST(label), TGUI_TRUE);
		tgui_widget_set_halign(TGUI_WIDGET_CAST(label), TGUI_ALIGN_CENTER);
		tgui_widget_set_valign(TGUI_WIDGET_CAST(label), TGUI_ALIGN_TOP);
	}
}

void tgui_button_set_icon(tgui_button_t *button, const char *icon_name) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_icon_get_type())) {
		// we already have an icon
		tgui_icon_set(TGUI_ICON_CAST(child), icon_name);
	} else {
		tgui_icon_t *icon = tgui_icon_new(icon_name);
		tgui_button_set_child(button, TGUI_WIDGET_CAST(icon));
		tgui_widget_set_hexpand(TGUI_WIDGET_CAST(icon), TGUI_TRUE);
		tgui_widget_set_vexpand(TGUI_WIDGET_CAST(icon), TGUI_TRUE);
		tgui_widget_set_halign(TGUI_WIDGET_CAST(icon), TGUI_ALIGN_CENTER);
		tgui_widget_set_valign(TGUI_WIDGET_CAST(icon), TGUI_ALIGN_TOP);
	}
}

void tgui_button_set_child(tgui_button_t *button, tgui_widget_t *child) {
	// destroy children if we already have one
	if (button->widget.children.first) {
		tgui_widget_destroy(TGUI_WIDGET_FROM_NODE(button->widget.children.first));
	}
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(button));
}

const char *tgui_button_get_text(tgui_button_t *button) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_label_get_type())) {
		return tgui_label_get_text(TGUI_LABEL_CAST(child));
	} else {
		return NULL;
	}
}

const char *tgui_button_get_icon(tgui_button_t *button) {
	(void)button;
	// TODO
	return NULL;
}

tgui_widget_t *tgui_button_get_child(tgui_button_t *button) {
	if (button->widget.children.first) {
		return TGUI_WIDGET_FROM_NODE(button->widget.children.first);
	} else {
		return NULL;
	}
}
