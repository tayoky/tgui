#include <widget.h>
#include <button.h>
#include <label.h>

static void tgui_button_calculate_sizes(tgui_widget_t *widget) {
	tgui_button_t *button = TGUI_BUTTON_CAST(widget);
	if (!button->widget.children.first) {
		button->widget.min_width = 0;
		button->widget.min_height = 0;
		button->widget.pref_width = 0;
		button->widget.pref_height = 0;
		return;
	}
	tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(button->widget.children.first);
	tgui_widget_calculate_sizes(child);
	button->widget.min_width  = child->min_width;
	button->widget.min_height = child->min_height;
	button->widget.pref_width  = child->min_width;
	button->widget.pref_height = child->pref_height;
}

static void tgui_button_allocate_space(tgui_widget_t *widget) {
	tgui_button_t *button = TGUI_BUTTON_CAST(widget);
	if (button->widget.children.count == 0) {
		return;
	}
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);


	tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(button->widget.children.first);
	tgui_widget_allocate_space(child, x, y, width, height);
}

static tgui_widget_class_t button_class = {
	.name = "button",
	.size = sizeof(tgui_button_t),
	.calculate_sizes = tgui_button_calculate_sizes,
	.allocate_space = tgui_button_allocate_space,
};

tgui_button_t *tgui_button_new(void){
	tgui_widget_t *widget = tgui_widget_new(&button_class);
	if (!widget) return NULL;

	tgui_button_t *button = TGUI_BUTTON_CAST(widget);
	return button;
}

void tgui_button_set_text(tgui_button_t *button, const char *text) {
	if (button->widget.children.first && tgui_widget_is_class(TGUI_WIDGET_FROM_NODE(button->widget.children.first), "label")) {
		// we already have a label
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(button->widget.children.first);
		tgui_label_set_text(TGUI_LABEL_CAST(child), text);
	} else {
		tgui_label_t *label = tgui_label_new(text);
		tgui_button_set_child(button, TGUI_WIDGET_CAST(label));
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
