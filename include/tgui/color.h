#ifndef TGUI_COLOR_H
#define TGUI_COLOR_H

#include <stdint.h>

typedef struct tgui_color {
	size_t ref_count;
	void *private;
} tgui_color_t;

tgui_color_t *tgui_color_new_rgb(uint8_t r, uint8_t g, uint8_t b);
tgui_color_t *tgui_color_new_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
tgui_color_t *tgui_color_ref(tgui_color_t *color);
void tgui_color_release(tgui_color_t *color);

#endif
