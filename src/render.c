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
	tgui_style_t *style = tgui_widget_get_current_style(widget);
	long frame_x = tgui_widget_get_frame_x(widget);
	long frame_y = tgui_widget_get_frame_y(widget);
	long frame_width = tgui_widget_get_frame_width(widget);
	long frame_height = tgui_widget_get_frame_height(widget);
	long border_x = tgui_widget_get_border_x(widget);
	long border_y = tgui_widget_get_border_y(widget);
	long border_width = tgui_widget_get_border_width(widget);
	long border_height = tgui_widget_get_border_height(widget);

	// first render background
	if (style->background_color) {
		tgui_render_rect(widget, style->background_color, frame_x, frame_y, frame_width, frame_height);
	}

	// TODO : more border styles
	if (style->border_style[TGUI_SIDE_LEFT] == TGUI_BORDER_SOLID) {
		tgui_render_rect(widget, style->border_color[TGUI_SIDE_LEFT], border_x, border_y, style->border_width[TGUI_SIDE_LEFT], border_height);
	}
	if (style->border_style[TGUI_SIDE_RIGHT] == TGUI_BORDER_SOLID) {
		unsigned int size = style->border_width[TGUI_SIDE_RIGHT];
		tgui_render_rect(widget, style->border_color[TGUI_SIDE_RIGHT], border_x + border_width - size, border_y, size, border_height);
	}
	if (style->border_style[TGUI_SIDE_TOP] == TGUI_BORDER_SOLID) {
		unsigned int size = style->border_width[TGUI_SIDE_TOP];
		tgui_render_rect(widget, style->border_color[TGUI_SIDE_TOP], border_x, border_y, border_width, size);
	}
	if (style->border_style[TGUI_SIDE_BOTTOM] == TGUI_BORDER_SOLID) {
		unsigned int size = style->border_width[TGUI_SIDE_BOTTOM];
		tgui_render_rect(widget, style->border_color[TGUI_SIDE_BOTTOM], border_x, border_y + border_height - size, border_width, size);
	}
}
