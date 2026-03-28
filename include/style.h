#ifndef TGUI_STYLE_H
#define TGUI_STYLE_H

#include "color.h"
#include "font.h"

typedef struct tgui_style {
	size_t ref_count;
	tgui_font_t *font;
	tgui_color_t *color;
	tgui_color_t *background_color;
	tgui_color_t *border_color[4];
	unsigned int border_width[4];
	char border_style[4];
	unsigned int font_size;
} tgui_style_t;

#define TGUI_SIDE_LEFT    1
#define TGUI_SIDE_RIGHT   2
#define TGUI_SIDE_TOP     3
#define TGUI_SIDE_BOTTOM  4
#define TGUI_SIDE_ALL     5

#define TGUI_BORDER_NONE   0
#define TGUI_BORDER_SOLILD 1
#define TGUI_BORDER_DASHED 2
#define TGUI_BORDER_DOTTED 3

tgui_style_t *tgui_style_new(void);
tgui_style_t *tgui_style_copy(tgui_style_t *style);
tgui_style_t *tgui_style_ref(tgui_style_t *style);
void tgui_style_release(tgui_style_t *style);
void tgui_style_set_border_width(tgui_style_t *style, int side, unsigned int width);
unsigned int tgui_style_get_border_width(tgui_style_t *style, int side);
void tgui_style_set_border_color(tgui_style_t *style, int side, tgui_color_t *color);
tgui_color_t *tgui_style_get_border_color(tgui_style_t *style, int side);
void tgui_style_set_border_style(tgui_style_t *style, int side, char border_style);
char tgui_style_get_border_style(tgui_style_t *style, int side);
void tgui_style_set_color(tgui_style_t *style, tgui_color_t *color);
tgui_color_t *tgui_style_get_color(tgui_style_t *style);
void tgui_style_set_background_color(tgui_style_t *style, tgui_color_t *color);
tgui_color_t *tgui_style_get_background_color(tgui_style_t *style);
void tgui_style_set_font(tgui_style_t *style, tgui_font_t *font);
tgui_font_t *tgui_style_get_font(tgui_style_t *style);
void tgui_style_set_font_size(tgui_style_t *style, unsigned int font_size);
unsigned int tgui_style_get_font_size(tgui_style_t *style);

#endif
