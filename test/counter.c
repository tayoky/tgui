#include <stdio.h>
#include <tgui/tgui.h>

int counter = 0;
tgui_label_t *label;

void update_label(void) {
	char text[32];
	sprintf(text, "%d", counter);
	tgui_label_set_text(label, text);
}

int plus_click(tgui_event_t *event) {
	(void)event;
	counter++;
	update_label();
	return TGUI_EVENT_HANDLED;
}

int minus_click(tgui_event_t *event) {
	(void)event;
	counter--;
	update_label();
	return TGUI_EVENT_HANDLED;
}

int main() {
	if (tgui_init() < 0) {
		puts("failed to init tgui");
		return 1;
	}

	tgui_window_t *window = tgui_window_new("tgui counter", 640, 480);

	label = tgui_label_new("0");
	tgui_button_t *plus = tgui_button_new();
	tgui_button_set_text(plus, "+");
	tgui_widget_set_callback(TGUI_WIDGET_CAST(plus), TGUI_EVENT_CLICK, plus_click, NULL);
	tgui_button_t *minus = tgui_button_new();
	tgui_button_set_text(minus, "-");
	tgui_widget_set_callback(TGUI_WIDGET_CAST(minus), TGUI_EVENT_CLICK, minus_click, NULL);

	// create the box with the counter the plus and the minus
	tgui_box_t *box = tgui_box_new();
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(label));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(plus));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(minus));
	tgui_window_set_child(window, TGUI_WIDGET_CAST(box));

	// add some style
	tgui_style_t *style = tgui_style_new();
	tgui_style_set_background_color(style, tgui_color_new_rgb(200, 200, 200));
	tgui_widget_add_style(TGUI_WIDGET_CAST(box), style);

	tgui_main();

	tgui_fini();
	return 0;
}
