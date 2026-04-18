#include <platform.h>
#include <stdlib.h>
#include <inputs.h>
#include <input.h>
#include <twm.h>
#include <gfx.h>

static gfx_t gfx;

typedef struct stanix_window {
	twm_window_t window;
	gfx_t *gfx;
	gfx_t *clip;
	long cursor_x;
	long cursor_y;
	long clip_x;
	long clip_y;
} stanix_window_t;

static tgui_surface_t *get_surface(twm_window_t window_id) {
	TGUI_LIST_FOREACH(node, tgui_get_surfaces()) {
		tgui_surface_t *surface = TGUI_CONTAINER_OF(node, tgui_surface_t, node);
		stanix_window_t *stanix_window = surface->private;
		if (stanix_window->window == window_id) {
			return surface;
		}
	}
	return NULL;
}

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

void send_button_event(tgui_surface_t *surface, int button, twm_event_input_t *input_event) {
	stanix_window_t *stanix_window = surface->private;
	if (input_event->key.flags & TWM_INPUT_PRESS) {
		tgui_input_click(surface, button, stanix_window->cursor_x, stanix_window->cursor_y);
	} else {
		tgui_input_unclick(surface, button, stanix_window->cursor_x, stanix_window->cursor_y);
	}
}

void send_key_event(tgui_surface_t *surface, twm_event_input_t *input_event) {
	long key;
	switch (input_event->key.key) {
	case INPUT_KEY_MOUSE_LEFT:
		key = TGUI_KEY_MOUSE_LEFT;
		break;
	case INPUT_KEY_MOUSE_MIDDLE:
		key = TGUI_KEY_MOUSE_MIDDLE;
		break;
	case INPUT_KEY_MOUSE_RIGHT:
		key = TGUI_KEY_MOUSE_RIGHT;
		break;
	case INPUT_KEY_ESC:
		key = TGUI_KEY_ESC;
		break;
	case INPUT_KEY_TAB:
		key = TGUI_KEY_TAB;
		break;
	case INPUT_KEY_BACKSPACE:
		key = TGUI_KEY_BACKSPACE;
		break;
	case INPUT_KEY_ENTER:
		key = TGUI_KEY_ENTER;
		break;
	case INPUT_KEY_DELETE:
		key = TGUI_KEY_DELETE;
		break;
	case INPUT_KEY_INSERT:
		key = TGUI_KEY_INSERT;
		break;
	case INPUT_KEY_HOME:
		key = TGUI_KEY_HOME;
		break;
	case INPUT_KEY_END:
		key = TGUI_KEY_END;
		break;
	case INPUT_KEY_ARROW_UP:
		key = TGUI_KEY_ARROW_UP;
		break;
	case INPUT_KEY_ARROW_DOWN:
		key = TGUI_KEY_ARROW_DOWN;
		break;
	case INPUT_KEY_ARROW_LEFT:
		key = TGUI_KEY_ARROW_LEFT;
		break;
	case INPUT_KEY_ARROW_RIGHT:
		key = TGUI_KEY_ARROW_RIGHT;
		break;
	case INPUT_KEY_PAGE_UP:
		key = TGUI_KEY_PAGE_UP;
		break;
	case INPUT_KEY_PAGE_DOWN:
		key = TGUI_KEY_PAGE_DOWN;
		break;
	case INPUT_KEY_LSHIFT:
		key = TGUI_KEY_LSHIFT;
		break;
	case INPUT_KEY_RSHIFT:
		key = TGUI_KEY_RSHIFT;
		break;
	case INPUT_KEY_LCRTL:
		key = TGUI_KEY_LCRTL;
		break;
	case INPUT_KEY_RCRTL:
		key = TGUI_KEY_RCRTL;
		break;
	case INPUT_KEY_LALT:
		key = TGUI_KEY_LALT;
		break;
	case INPUT_KEY_RALT:
		key = TGUI_KEY_RALT;
		break;
	case INPUT_KEY_ALTGR:
		key = TGUI_KEY_ALTGR;
		break;
	case INPUT_KEY_NUM_LOCK:
		key = TGUI_KEY_NUM_LOCK;
		break;
	case INPUT_KEY_SCROLL_LOCK:
		key = TGUI_KEY_SCROLL_LOCK;
		break;
	case INPUT_KEY_CAPS_LOCK:
		key = TGUI_KEY_CAPS_LOCK;
		break;
	case INPUT_KEY_F1:
		key = TGUI_KEY_F1;
		break;
	case INPUT_KEY_F2:
		key = TGUI_KEY_F2;
		break;
	case INPUT_KEY_F3:
		key = TGUI_KEY_F3;
		break;
	case INPUT_KEY_F4:
		key = TGUI_KEY_F4;
		break;
	case INPUT_KEY_F5:
		key = TGUI_KEY_F5;
		break;
	case INPUT_KEY_F6:
		key = TGUI_KEY_F6;
		break;
	case INPUT_KEY_F7:
		key = TGUI_KEY_F7;
		break;
	case INPUT_KEY_F8:
		key = TGUI_KEY_F8;
		break;
	case INPUT_KEY_F9:
		key = TGUI_KEY_F9;
		break;
	case INPUT_KEY_F10:
		key = TGUI_KEY_F10;
		break;
	case INPUT_KEY_F11:
		key = TGUI_KEY_F11;
		break;
	case INPUT_KEY_F12:
		key = TGUI_KEY_F12;
		break;
	case INPUT_KEY_GUI:
		key = TGUI_KEY_GUI;
		break;
	case INPUT_KEY_VOLUME_UP:
		key = TGUI_KEY_VOLUME_UP;
		break;
	case INPUT_KEY_VOLUME_DOWN:
		key = TGUI_KEY_VOLUME_DOWN;
		break;
	default:
		key = input_event->key.key;
		break;
	}

	if (input_event->key.flags & TWM_INPUT_PRESS) {
		tgui_input_key_press(surface, input_event->key.scancode, key);
	} else {
		tgui_input_key_release(surface, input_event->key.scancode, key);
	}
}

void tgui_platform_handle_event(void) {
	twm_event_t *event = twm_poll_event();
	switch (event->type) {
	case TWM_EVENT_INPUT:;
		twm_event_input_t *input_event = (twm_event_input_t *)event;
		tgui_surface_t *surface = get_surface(input_event->window);
		stanix_window_t *stanix_window = surface->private;
		switch (input_event->type) {
		case TWM_INPUT_MOVE:
			stanix_window->cursor_x = input_event->move.abs_x;
			stanix_window->cursor_y = input_event->move.abs_y;
			tgui_input_move(surface, input_event->move.abs_x, input_event->move.abs_y);
			break;
		case TWM_INPUT_KEY:
			switch (input_event->key.scancode) {
			case INPUT_KEY_MOUSE_LEFT:
				send_button_event(surface, TGUI_BUTTON_LEFT, input_event);
				break;
			case INPUT_KEY_MOUSE_RIGHT:
				send_button_event(surface, TGUI_BUTTON_RIGHT, input_event);
				break;
			case INPUT_KEY_MOUSE_MIDDLE:
				send_button_event(surface, TGUI_BUTTON_MIDDLE, input_event);
				break;
			default:
				send_key_event(surface, input_event);
				break;
			}
		}
		break;
	default:
		break;
	}
	free(event);
}

int tgui_platform_create_window(tgui_window_t *window) {
	stanix_window_t *stanix_window = malloc(sizeof(stanix_window_t));
	stanix_window->window = twm_create_window(window->title, window->surface.width, window->surface.height);
	if (stanix_window->window < 0) {
		free(stanix_window);
		return -1;
	}
	stanix_window->gfx = twm_get_window_gfx(stanix_window->window);
	window->surface.private = stanix_window;
	return 0;
}

void tgui_platform_close_window(tgui_window_t *window) {
	tgui_platform_close_surface(&window->surface);
}

int tgui_platform_create_surface(tgui_surface_t *surface, tgui_surface_t *parent) {
	(void)parent;
	stanix_window_t *stanix_window = malloc(sizeof(stanix_window_t));
	stanix_window->window = twm_create_window("surface", surface->width, surface->height);
	if (stanix_window->window < 0) {
		free(stanix_window);
		return -1;
	}
	stanix_window->gfx = twm_get_window_gfx(stanix_window->window);
	surface->private = stanix_window;
	return 0;
}

void tgui_platform_close_surface(tgui_surface_t *surface) {
	stanix_window_t *stanix_window = surface->private;
	gfx_free(stanix_window->gfx);
	twm_destroy_window(stanix_window->window);
	free(stanix_window);
}

void tgui_platform_push_surface(tgui_surface_t *surface) {
	stanix_window_t *stanix_window = surface->private;
	long width  = surface->inval_end_x - surface->inval_start_x;
	long height = surface->inval_end_y - surface->inval_start_y;
	gfx_push_rect(stanix_window->gfx, surface->inval_start_x, surface->inval_start_y, width, height);
	twm_redraw_window(stanix_window->window, surface->inval_start_x, surface->inval_start_y, width, height);
}

void tgui_platform_set_surface_visible(tgui_surface_t *surface, int visible) {
	stanix_window_t *stanix_window = surface->private;
	if (visible) {
		twm_set_window_attr(stanix_window->window, TWM_ADD_ATTR, TWM_ATTR_SHOW);
	} else {
		twm_set_window_attr(stanix_window->window, TWM_REMOVE_ATTR, TWM_ATTR_SHOW);
	}
}

void tgui_platform_set_surface_position(tgui_surface_t *surface, long x, long y) {
}

void tgui_platform_new_color(tgui_color_t *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	color->private = (void *)(uintptr_t)gfx_color_rgba(&gfx, r, g, b, a);
}

void tgui_platform_free_color(tgui_color_t *color) {
	(void)color;
}

int tgui_platform_load_font(tgui_font_t *font, tgui_sized_font_t *sized) {
	font_t *twm_font = gfx_load_font(font->name);
	if (!twm_font) return -1;
	font->private = twm_font;
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font, tgui_sized_font_t *sized) {
	gfx_free_font(font->private);
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	return gfx_string_width(tgui_widget_get_font(widget)->private, text);
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	return gfx_string_height(tgui_widget_get_font(widget)->private, text);
}

int tgui_platform_load_image(tgui_image_t *image) {
	texture_t *texture = gfx_load_texture(&gfx, image->filename);
	if (!texture) return -1;
	image->private = texture;
	image->width  = texture->width;
	image->height = texture->height;
	return 0;
}

void tgui_platform_free_image(tgui_image_t *image) {
	gfx_free_texture(image->private);
}

void tgui_platform_render_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height) {
	stanix_window_t *stanix_window = surface->private;
	color_t stanix_color = (color_t)(uintptr_t)color->private;
	gfx_draw_rect(stanix_window->clip, stanix_color, x - stanix_window->clip_x, y - stanix_window->clip_y, width, height);
}

void tgui_platform_render_rounded_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, char corners, unsigned int rounded_size) {
	stanix_window_t *stanix_window = surface->private;
	color_t stanix_color = (color_t)(uintptr_t)color->private;
	gfx_draw_rounded_rect(stanix_window->clip, stanix_color, x - stanix_window->clip_x, y - stanix_window->clip_y, width, height, corners, rounded_size);

}

void tgui_platform_render_rounded_rect_outline(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, unsigned int border_size, unsigned int rounded_size) {
}

void tgui_platform_render_text(tgui_surface_t *surface, tgui_widget_t *widget, long x, long y, const char *text) {
	stanix_window_t *stanix_window = surface->private;
	color_t color = (color_t)(uintptr_t)tgui_widget_get_color(widget)->private;
	gfx_draw_string(stanix_window->clip, tgui_widget_get_font(widget)->private, color, x - stanix_window->clip_x, y - stanix_window->clip_y, text);
}


void tgui_platform_render_image(tgui_surface_t *surface, long x, long y, tgui_image_t *image) {
	stanix_window_t *stanix_window = surface->private;
	gfx_draw_texture_alpha(stanix_window->clip, image->private, x - stanix_window->clip_x, y - stanix_window->clip_y);
}

void tgui_platform_set_clip(tgui_surface_t *surface, long x, long y, long width, long height) {
	stanix_window_t *stanix_window = surface->private;
	stanix_window->clip_x = x;
	stanix_window->clip_y = y;
	gfx_free(stanix_window->clip);
	stanix_window->clip = gfx_create_clip(stanix_window->gfx, x, y, width, height);
}

void tgui_platform_start_dragging(tgui_window_t *window, long mouse_x, long mouse_y) {
	stanix_window_t *stanix_window = window->surface.private;
	twm_start_dragging(stanix_window->window, -mouse_x, -mouse_y);
}

void tgui_platform_canva_create(tgui_canva_t *canva) {
	tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(canva));
	if (!surface) return;
	stanix_window_t *stanix_window = surface->private;
	canva->private = gfx_create_buffer(stanix_window->gfx, canva->widget.width, canva->widget.height);
}

void tgui_platform_canva_destroy(tgui_canva_t *canva) {
	gfx_free(canva->private);
}

void tgui_platform_push_canva(tgui_canva_t *canva) {
	tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(canva));
	stanix_window_t *stanix_window = surface->private;
	gfx_draw_buffer(stanix_window->clip, canva->widget.x - stanix_window->clip_x, canva->widget.y - stanix_window->clip_y, canva->private);
}

int tgui_platform_get_fd(void) {
	return twm_get_fd();
}
