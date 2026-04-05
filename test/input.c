#include <stdio.h>
#include <tgui/tgui.h>

void add_element(tgui_box_t *box, tgui_widget_t *widget) {
	char text[256];
	sprintf(text, "this is is a %s", widget->class->name);
	tgui_label_t *label = tgui_label_new(text);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(label));
	tgui_widget_set_hexpand(widget, TGUI_TRUE);
	tgui_box_append_widget(box, widget);
}

void add_text(tgui_box_t *box, const char *text) {
	tgui_label_t *label = tgui_label_new(text);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(label));
}

void add_button(tgui_box_t *box, int align) {
	tgui_button_t *button = tgui_button_new();
	tgui_button_set_text(button, "button");
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
	tgui_widget_set_halign(TGUI_WIDGET_CAST(button), align);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(button));
}

tgui_widget_t *input_tab() {
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);

	tgui_button_t *button = tgui_button_new();
	tgui_button_set_text(button, "button");
	add_element(box, TGUI_WIDGET_CAST(button));

	tgui_slider_t *slider = tgui_slider_new(TGUI_ORIENTATION_HORIZONTAL);
	tgui_slider_set_value(slider, 0.3f);
	add_element(box, TGUI_WIDGET_CAST(slider));
	return TGUI_WIDGET_CAST(box);
}

tgui_widget_t *align_tab() {
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);

	add_text(box, "left");
	add_button(box, TGUI_ALIGN_LEFT);
	add_text(box, "center");
	add_button(box, TGUI_ALIGN_CENTER);
	add_text(box, "right");
	add_button(box, TGUI_ALIGN_RIGHT);
	add_text(box, "fill");
	add_button(box, TGUI_ALIGN_FILL);

	return TGUI_WIDGET_CAST(box);
}

int main() {
	if (tgui_init() < 0) {
		puts("fail to init twm");
		return 1;
	}

	tgui_window_t *window = tgui_window_new("tgui input test", 640, 480);
	
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_window_set_child(window, TGUI_WIDGET_CAST(box));

	tgui_stack_switcher_t *tabs = tgui_stack_switcher_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(tabs), TGUI_TRUE);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(tabs));

	tgui_stack_t *stack = tgui_stack_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(stack), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(stack), TGUI_TRUE);
	tgui_stack_switcher_set_stack(tabs, stack);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(stack));

	tgui_stack_add_child(stack, input_tab(), "inputs");
	tgui_stack_add_child(stack, align_tab(), "aligns");
	tgui_stack_switcher_update(tabs);
	tgui_main();
	tgui_fini();
	return 0;
}
