#include <widget.h>
#include <button.h>
#include <label.h>

static tgui_widget_class_t button_class = {
	.name = "button",
	.size = sizeof(tgui_button_t),
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space = tgui_container_single_allocate_space,
};

tgui_button_t *tgui_button_new(void){
	tgui_widget_t *widget = tgui_widget_new(&button_class);
	if (!widget) return NULL;

	tgui_button_t *button = TGUI_BUTTON_CAST(widget);
	return button;
}

void tgui_button_set_text(tgui_button_t *button, const char *text) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_class(child, "label")) {
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

void tgui_button_set_icon(tgui_button_t *button, const char *icon) {
	// TODO : images and stuff
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
	if (tgui_widget_is_class(child, "label")) {
		return tgui_label_get_text(TGUI_LABEL_CAST(child));
	} else {
		return NULL;
	}
}

const char *tgui_button_get_icon(tgui_button_t *button);

tgui_widget_t *tgui_button_get_child(tgui_button_t *button) {
	if (button->widget.children.first) {
		return TGUI_WIDGET_FROM_NODE(button->widget.children.first);
	} else {
		return NULL;
	}
}
