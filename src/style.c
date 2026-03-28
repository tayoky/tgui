#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <style.h>

tgui_style_t *tgui_style_new(void) {
	tgui_style_t *style = malloc(sizeof(tgui_style_t));
	memset(style, 0, sizeof(tgui_style_t));
	// default font size
	style->font_size = 12;
	return style;
}

tgui_style_t *tgui_style_copy(tgui_style_t *style) {
	tgui_style_t *new_style = malloc(sizeof(tgui_style_t));
	memcpy(new_style, style, sizeof(tgui_style_t));
	new_style->ref_count = 0;
	return new_style;
}

tgui_style_t *tgui_style_ref(tgui_style_t *style) {
	if (style) style->ref_count++;
	return style;
}

void tgui_style_release(tgui_style_t *style)  {
	if (style->ref_count > 1) {
		style->ref_count--;
		return;
	}
	for (int i=0; i<4; i++) {
		tgui_color_release(style->border_color[i]);
	}
	tgui_font_release(style->font);
	tgui_color_release(style->background_color);
	tgui_color_release(style->color);
	free(style);
}

void tgui_style_set_border_width(tgui_style_t *style, int side, unsigned int width) {
	if (side == TGUI_SIDE_ALL) {
		for (int i=0; i<4;i++) {
			style->border_width[i] = width;
		}
	} else {
		style->border_width[side] = width;
	}
}

unsigned int tgui_style_get_border_width(tgui_style_t *style, int side) {
	if (!style) return 0;
	return style->border_width[side];
}

void tgui_style_set_border_color(tgui_style_t *style, int side, tgui_color_t *color) {
	if (side == TGUI_SIDE_ALL) {
		for (int i=0; i<4;i++) {
			tgui_color_release(style->border_color[i]);
			style->border_color[i] = tgui_color_ref(color);
		}
	} else {
		tgui_color_release(style->border_color[side]);
		style->border_color[side] = tgui_color_ref(color);
	}
}

tgui_color_t *tgui_style_get_border_color(tgui_style_t *style, int side) {
	if (!style) return NULL;
	return style->border_color[side];
}

void tgui_style_set_border_style(tgui_style_t *style, int side, char border_style) {
	if (side == TGUI_SIDE_ALL) {
		for (int i=0; i<4;i++) {
			style->border_style[i] = border_style;
		}
	} else {
		style->border_style[side] = border_style;
	}
}

char tgui_style_get_border_style(tgui_style_t *style, int side) {
	if (!style) return TGUI_BORDER_NONE;
	return style->border_style[side];
}

void tgui_style_set_color(tgui_style_t *style, tgui_color_t *color) {
	if (style->color == color) return;
	tgui_color_release(style->color);
	style->color = tgui_color_ref(color);
}

tgui_color_t *tgui_style_get_color(tgui_style_t *style) {
	if (!style) return NULL;
	return style->color;
}

void tgui_style_set_background_color(tgui_style_t *style, tgui_color_t *color) {
	if (style->background_color == color) return;
	tgui_color_release(style->background_color);
	style->background_color = tgui_color_ref(color);
}

tgui_color_t *tgui_style_get_background_color(tgui_style_t *style) {
	if (!style) return NULL;
	return style->background_color;
}

void tgui_style_set_font(tgui_style_t *style, tgui_font_t *font) {
	if (style->font == font) return;
	tgui_font_release(style->font);
	style->font = tgui_font_ref(style->font);
}

tgui_font_t *tgui_style_get_font(tgui_style_t *style) {
	if (!style) return NULL;
	return style->font;
}

void tgui_style_set_font_size(tgui_style_t *style, unsigned int font_size) {
	style->font_size = font_size;
}

unsigned int tgui_style_get_font_size(tgui_style_t *style) {
	if (!style) return 0;
	return style->font_size;
}
