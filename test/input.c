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

	tgui_button_t *button = tgui_button_new();
	tgui_button_set_text(button, "button");
	add_element(box, TGUI_WIDGET_CAST(button));

	tgui_slider_t *slider = tgui_slider_new(TGUI_ORIENTATION_HORIZONTAL);
	tgui_slider_set_value(slider, 0.3f);
	add_element(box, TGUI_WIDGET_CAST(slider));

	tgui_main();
	tgui_fini();
	return 0;
}
