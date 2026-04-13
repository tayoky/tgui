#include <widget.h>
#include <style.h>
#include <theme.h>

int tgui_theme_load(void) {
	// colors
	tgui_color_t *bg = tgui_color_new_rgb(200, 200, 200);
	tgui_color_t *color = tgui_color_new_rgb(0, 0, 0);
	tgui_color_t *button_color = tgui_color_new_rgb(150, 150, 150);
	tgui_color_t *border_color = tgui_color_new_rgb(100, 100, 100);
	tgui_color_t *accent = tgui_color_new_rgb(0, 150, 0);
	unsigned int border_width = 2;
	unsigned int padding      = 4;

	// window
	tgui_style_t *window_style = tgui_style_new();
	tgui_style_set_background_color(window_style, bg);
	tgui_style_set_border_style(window_style, TGUI_SIDE_ALL, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(window_style, TGUI_SIDE_ALL, border_width);
	tgui_style_set_border_color(window_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_color(window_style, color);
	tgui_widget_set_default_style(window_style, "window");
	tgui_widget_set_default_style(window_style, "surface");

	// box
	tgui_style_t *box_style = tgui_style_new();
	tgui_style_set_border_style(box_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_widget_set_default_style(box_style, "box");

	// buttons
	tgui_style_t *button_style = tgui_style_new();
	tgui_style_set_rounded_corners(button_style, TGUI_CORNER_ALL);
	tgui_style_set_background_color(button_style, button_color);
	tgui_style_set_border_style(button_style, TGUI_SIDE_ALL, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(button_style, TGUI_SIDE_ALL, border_width);
	tgui_style_set_border_color(button_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_padding(button_style, TGUI_SIDE_ALL, padding);
	tgui_widget_set_default_style(button_style, "button");

	// labels
	tgui_style_t *label_style = tgui_style_new();
	tgui_style_set_rounded_corners(label_style, 0);
	tgui_style_set_border_style(label_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_style_set_padding(label_style, TGUI_SIDE_ALL, 0);
	tgui_widget_set_default_style(label_style, "label");

	// labels title
	tgui_style_t *title_style = tgui_style_new();
	tgui_style_set_font_size(title_style, 20);
	tgui_style_set_rounded_corners(title_style, 0);
	tgui_style_set_border_style(title_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_style_set_padding(title_style, TGUI_SIDE_ALL, 0);
	tgui_widget_set_default_style(title_style, "title");

	// separators
	tgui_style_t *separator_style = tgui_style_new();
	tgui_style_set_border_style(separator_style, TGUI_SIDE_LEFT, TGUI_BORDER_SOLID);
	tgui_style_set_border_style(separator_style, TGUI_SIDE_TOP, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(separator_style, TGUI_SIDE_ALL, border_width);
	tgui_style_set_border_color(separator_style, TGUI_SIDE_ALL, border_color);
	tgui_widget_set_default_style(separator_style, "separator");

	// text input
	tgui_style_t *text_style = tgui_style_new();
	tgui_style_set_rounded_corners(text_style, TGUI_CORNER_ALL);
	//tgui_style_set_background_color(text_style, button_color);
	tgui_style_set_border_style(text_style, TGUI_SIDE_ALL, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(text_style, TGUI_SIDE_ALL, border_width);
	tgui_style_set_border_color(text_style, TGUI_SIDE_ALL, border_color);
	tgui_style_set_padding(text_style, TGUI_SIDE_ALL, padding);
	tgui_widget_set_default_style(text_style, "text");

	// slider
	tgui_style_t *slider_style = tgui_style_new();
	tgui_style_set_background_color(slider_style, border_color);
	tgui_widget_set_default_style(slider_style, "slider");

	// title bar
	tgui_style_t *title_bar_style = tgui_style_new();
	tgui_style_set_border_style(title_bar_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_style_set_border_style(title_bar_style, TGUI_SIDE_BOTTOM, TGUI_BORDER_SOLID);
	tgui_style_set_border_width(title_bar_style, TGUI_SIDE_ALL, border_width);
	tgui_style_set_border_color(title_bar_style, TGUI_SIDE_ALL, border_color);
	tgui_widget_set_default_style(title_bar_style, "title bar");


	// icons
	tgui_style_t *icon_style = tgui_style_new();
	tgui_style_set_border_style(icon_style, TGUI_SIDE_ALL, TGUI_BORDER_NONE);
	tgui_style_set_padding(icon_style, TGUI_SIDE_ALL, 0);
	tgui_widget_set_default_style(icon_style, "icon");

	return 0;
}
