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

int tgui_platform_create_surface(tgui_surface_t *surface, tgui_surface_t *parent) {
}

void tgui_platform_close_surface(tgui_surface_t *surface) {
}

void tgui_platform_push_surface(tgui_surface_t *surface) {
}

void tgui_platform_set_surface_visible(tgui_surface_t *surface, int visible) {
}

void tgui_platform_set_surface_position(tgui_surface_t *surface, long x, long y) {
}

void tgui_platform_grab_surface(tgui_surface_t *surface) {
}

void tgui_platform_ungrab_surface(tgui_surface_t *surface) {
}

void tgui_platform_new_color(tgui_color_t *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
}

void tgui_platform_free_color(tgui_color_t *color) {
}

int tgui_platform_load_font(tgui_font_t *font, tgui_sized_font_t *sized) {
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font, tgui_sized_font_t *sized) {
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	return widget->font_size * strlen(text);
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	return widget->font_size;
}

int tgui_platform_load_image(tgui_image_t *image) {
	return 0;
}

void tgui_platform_free_image(tgui_image_t *image) {
}

void tgui_platform_render_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height) {
}

void tgui_platform_render_rounded_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, char corners, unsigned int rounded_size) {
}

void tgui_platform_render_rounded_rect_outline(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, unsigned int border_size, unsigned int rounded_size) {
}

void tgui_platform_render_text(tgui_surface_t *surface, tgui_widget_t *widget, long x, long y, const char *text) {
}

void tgui_platform_render_image(tgui_surface_t *surface, long x, long y, tgui_image_t *image) {
}

void tgui_platform_set_clip(tgui_surface_t *surface, long x, long y, long width, long height) {
}

void tgui_platform_start_dragging(tgui_window_t *window, long mouse_x, long mouse_y) {
}

void tgui_platform_canva_create(tgui_canva_t *canva) {
}

void tgui_platform_canva_destroy(tgui_canva_t *canva) {
}

void tgui_platform_push_canva(tgui_canva_t *canva) {
}

int tgui_platform_get_fd(void) {
}

void tgui_platform_register_handler(int (*handler)(void *event, void *data), void *data) {
}
