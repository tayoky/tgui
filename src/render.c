#include <render.h>
#include <surface.h>
#include <platform.h>

void tgui_render_rect(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height) {
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	if (!surface) return;
	tgui_platform_render_rect(surface, color, x * surface->scaling, y * surface->scaling, width *surface->scaling, height * surface->scaling);
}

void tgui_render_rounded_rect(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height, char corners, unsigned int rayon) {
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	if (!surface) return;
	tgui_platform_render_rounded_rect(surface, color, x * surface->scaling, y * surface->scaling, width *surface->scaling, height * surface->scaling, corners, rayon * surface->scaling);
}

void tgui_render_rounded_rect_outline(tgui_widget_t *widget, tgui_color_t *color, long x, long y, long width, long height, unsigned int border_size, unsigned int rayon) {
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	if (!surface) return;
	tgui_platform_render_rounded_rect_outline(surface, color, x * surface->scaling, y * surface->scaling, width *surface->scaling, height * surface->scaling, border_size * surface->scaling, rayon * surface->scaling);
}

void tgui_render_text(tgui_widget_t *widget, long x, long y, const char *text) {
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	if (!surface) return;
	tgui_platform_render_text(surface, widget, x * surface->scaling, y * surface->scaling, text);
}

void tgui_render_set_clip(tgui_surface_t *surface, tgui_rect_t *rect) {
	if (!surface) return;
	long x = rect->start_x * surface->scaling;
	long y = rect->start_y * surface->scaling;
	long width  = (rect->end_x - rect->start_x) * surface->scaling;
	long height = (rect->end_y - rect->start_y) * surface->scaling;
	surface->clip = *rect;
	tgui_platform_set_clip(surface, x, y, width, height);
}

void tgui_render_get_clip(tgui_surface_t *surface, tgui_rect_t *rect) {
	if (!surface) return;
	*rect = surface->clip;
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
		if (style->rounded_corners) {
			tgui_render_rounded_rect(widget, style->background_color, frame_x, frame_y, frame_width, frame_height, style->rounded_corners, style->rounded_size);
		} else {
			tgui_render_rect(widget, style->background_color, frame_x, frame_y, frame_width, frame_height);
		}
	}

	// TODO : better rounded borders
	if (style->border_style[TGUI_SIDE_LEFT] && style->rounded_corners) {
		unsigned int border_size = style->border_width[TGUI_SIDE_LEFT];
		tgui_render_rounded_rect_outline(widget, style->border_color[TGUI_SIDE_LEFT], border_x, border_y, border_width, border_height, border_size, style->rounded_size + border_size);
	} else {
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
}
