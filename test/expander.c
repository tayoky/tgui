#include <stdio.h>
#include <tgui/tgui.h>

int main() {
	if (tgui_init() < 0) {
		puts("failed to init tgui");
		return 1;
	}

	tgui_window_t *window = tgui_window_new("tgui expander", 640, 480);

	tgui_label_t *label = tgui_label_new("some very long text");

	// create an expander
	tgui_expander_t *expander = tgui_expander_new();
	tgui_expander_set_title_text(expander, "expander");
	tgui_expander_set_child(expander, TGUI_WIDGET_CAST(label));
	tgui_window_set_child(window, TGUI_WIDGET_CAST(expander));

	// add some style
	tgui_style_t *style = tgui_style_new();
	tgui_style_set_background_color(style, tgui_color_new_rgb(200, 200, 200));
	tgui_widget_add_style(TGUI_WIDGET_CAST(expander), style);

	tgui_main();

	tgui_fini();
	return 0;
}
