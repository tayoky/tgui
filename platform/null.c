#include <paltform.h>
#include <string.h>

int tgui_platform_init(void) {
	return 0;
}

void tgui_platform_fini(void) {
}

void tgui_platform_handle_event(void) {
}

int tgui_platform_create_window(tgui_window_t *window) {
	return 0;
}

void tgui_platform_close_window(tgui_window_t *window) {
}

void tgui_platform_push_window(tgui_window_t *window) {
}

void tgui_platform_new_color(tgui_color_t *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
}

void tgui_platform_free_color(tgui_color_t *color) {
}

int tgui_platform_load_font(tgui_font_t *font) {
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font) {
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	return widget->font_size * strlen(text);
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	return widget->font_size;

void tgui_platform_render_rect(tgui_window_t *window, tgui_color_t *color, long x, long y, long width, long height) {
}

void tgui_platform_render_text(tgui_window_t *window, tgui_widget_t *widget, long x, long y, const char *text) {
}

void tgui_platform_set_clip(tgui_window_t *window, long x, long y, long width, long height) {
}
