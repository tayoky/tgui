#include <render.h>
#include <window.h>
#include <platform.h>

void tgui_render_rect(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height) {
	tgui_window_t *window = tgui_widget_get_window(widget);
	if (!window) return;
	tgui_platform_render_rect(window, color, x, y, width, height);
}

void tgui_render_text(tgui_widget_t *widget, long x, long y, const char *text) {
	tgui_window_t *window = tgui_widget_get_window(widget);
	if (!window) return;
	tgui_platform_render_text(window, widget, x, y, text);
}

void tgui_render_widget_base(tgui_widget_t *widget) {
	tgui_style_t style;
	tgui_widget_get_current_style(widget, &style);

	// first render background
	if (style.background_color) {
		long x = tgui_widget_get_frame_x(widget);
		long y = tgui_widget_get_frame_y(widget);
		long width = tgui_widget_get_frame_width(widget);
		long height = tgui_widget_get_frame_height(widget);
		tgui_render_rect(widget, style.background_color, x, y, width, height);
	}

	// TODO : border
}
