#include <style.h>
#include <theme.h>

int tgui_theme_load(void) {
	// colors
	tgui_color_t *bg = tgui_color_new_rgb(200, 200, 200);
	tgui_color_t *color = tgui_color_new_rgb(0, 0, 0);
	tgui_color_t *button_color = tgui_color_new_rgb(150, 150, 150);
	tgui_color_t *border_color = tgui_color_new_rgb(100, 100, 100);
	tgui_color_t *accent = tgui_color_new_rgb(0, 150, 0);

	// window
	tgui_style_t *window_style = tgui_style_new();
	tgui_style_set_background_color(window_style, bg);
	tgui_style_set_color(window_style, color);
	tgui_style_set_default(window_style, "window");

	// buttons
	tgui_style_t *button_style = tgui_style_new();
	tgui_style_set_background_color(button_style, button_color);
	tgui_style_set_border_style(button_style, TGUI_SIDE_ALL, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(button_style, TGUI_SIDE_ALL, 1);
	tgui_style_set_border_color(button_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_default(button_style, "button");

	// labels
	tgui_style_t *label_style = tgui_style_new();
	tgui_style_set_border_style(label_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_style_set_default(label_style, "label");

	// separators
	tgui_style_t *separator_style = tgui_style_new();
	tgui_style_set_border_style(separator_style, TGUI_SIDE_LEFT, TGUI_BORDER_SOLID);
	tgui_style_set_border_style(separator_style, TGUI_SIDE_TOP, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(separator_style, TGUI_SIDE_ALL, 1);
	tgui_style_set_border_color(separator_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_default(separator_style, "separator");

	// text input
	tgui_style_t *text_style = tgui_style_new();
	//tgui_style_set_background_color(text_style, button_color);
	tgui_style_set_border_style(text_style, TGUI_SIDE_ALL, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(text_style, TGUI_SIDE_ALL, 1);
	tgui_style_set_border_color(text_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_default(text_style, "text");

	// slider
	tgui_style_t *slider_style = tgui_style_new();
	tgui_style_set_background_color(slider_style, border_color);
	tgui_style_set_default(slider_style, "slider");

	return 0;
}
