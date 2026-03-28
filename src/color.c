#include <platform.h>
#include <color.h>
#include <stdlib.h>

tgui_color_t *tgui_color_new_rgb(uint8_t r, uint8_t g, uint8_t b) {
	return tgui_color_new_rgba(r, g, b, 255);
}

tgui_color_t *tgui_color_new_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	tgui_color_t *color = malloc(sizeof(tgui_color_t));
	color->ref_count = 0;
	tgui_platform_new_color(color, r, g, b, a);
	return color;
}

tgui_color_t *tgui_color_ref(tgui_color_t *color) {
	if (color) color->ref_count++;
	return color;
}

void tgui_color_release(tgui_color_t *color) {
	if (!color) return;
	if (color->ref_count > 1) {
		color->ref_count--;
		return;
	}
	tgui_platform_free_color(color);
	free(color);
}
