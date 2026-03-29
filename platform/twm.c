#include <platform.h>
#include <stdlib.h>
#include <twm.h>
#include <gfx.h>

static gfx_t gfx;

typedef struct stanix_window {
	twm_window_t window;
	gfx_t *gfx;
} stanix_window_t;

int tgui_platform_init(void) {
	if (twm_init(NULL) < 0) return -1;
	twm_fb_info_t fb_info;
	twm_get_screen_fb(0, &fb_info);
	gfx.bpp = fb_info.bpp;
	gfx.red_mask_shift   = fb_info.red_mask_shift;
	gfx.red_mask_size    = fb_info.red_mask_size;
	gfx.green_mask_shift = fb_info.green_mask_shift;
	gfx.green_mask_size  = fb_info.green_mask_size;
	gfx.blue_mask_shift  = fb_info.blue_mask_shift;
	gfx.blue_mask_size   = fb_info.blue_mask_size;
	return 0;
}

void tgui_platform_fini(void) {
	twm_fini();
}

void tgui_platform_handle_event(void) {
	twm_event_t *event = twm_poll_event();
	switch (event->type) {
	default:
		break;
	}
}

int tgui_platform_create_window(tgui_window_t *window) {
	stanix_window_t *stanix_window = malloc(sizeof(stanix_window_t));
	stanix_window->window = twm_create_window("tgui window", window->widget.width, window->widget.height);
	if (stanix_window->window) {
		free(stanix_window);
		return -1;
	}
	stanix_window->gfx = twm_get_window_gfx(stanix_window->window);
	window->private = stanix_window;
	return 0;
}

void tgui_platform_close_window(tgui_window_t *window) {
	stanix_window_t *stanix_window = window->private;
	gfx_free(stanix_window->gfx);
	twm_destroy_window(stanix_window->window);
	free(stanix_window);
}

void tgui_platform_push_window(tgui_window_t *window) {
	stanix_window_t *stanix_window = window->private;
	twm_redraw_window(stanix_window->window, 0, 0, TWM_WHOLE_WIDTH, TWM_WHOLE_HEIGHT);
}

void tgui_platform_new_color(tgui_color_t *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	color->private = (void*)(uintptr_t)gfx_color_rgba(&gfx, r, g, b, a);
}

void tgui_platform_free_color(tgui_color_t *color) {
	(void)color;
}

int tgui_platform_load_font(tgui_font_t *font) {
	font_t *twm_font = gfx_load_font(font->name);
	if (!twm_font) return -1;
	font->private = twm_font;
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font) {
	gfx_free_font(font->private);
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	return gfx_string_width(tgui_widget_get_font(widget)->private, text);
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	return gfx_string_height(tgui_widget_get_font(widget)->private, text);
}

void tgui_platform_render_rect(tgui_window_t *window, tgui_color_t *color, long x, long y, long width, long height) {
	stanix_window_t *stanix_window = window->private;
	color_t stanix_color = (color_t)(uintptr_t)color->private;
	gfx_draw_rect(stanix_window->gfx, stanix_color, x, y, width, height);
}

void tgui_platform_render_text(tgui_window_t *window, tgui_widget_t *widget, long x, long y, const char *text) {
	stanix_window_t *stanix_window = window->private;
	color_t color = (color_t)(uintptr_t)tgui_widget_get_color(widget)->private;
	gfx_draw_string(stanix_window->gfx, tgui_widget_get_font(widget)->private, color, x, y, text);
}
