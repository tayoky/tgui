#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <platform.h>
#include <inputs.h>
#include <list.h>
#include <stdlib.h>

typedef struct x11_window {
	Window window;
	Pixmap pixmap;
	GC gc;
	XftDraw *draw;
} x11_window_t;

typedef struct x11_font {
	tgui_list_t cache;
} x11_font_t;

typedef struct font_cache {
	tgui_list_node_t node;
	unsigned int size;
	XftFont *font;
} font_cache_t;

static Display *display;
static Visual *visual;
static Colormap colormap;
static int screen;

#define COLOR2XFT(color) (XftColor*)(color->private)

static tgui_window_t *get_window(Window window_id) {
	TGUI_LIST_FOREACH(node, tgui_get_windows()) {
		tgui_window_t *window = TGUI_CONTAINER_OF(node, tgui_window_t, node);
		x11_window_t *x11_window = window->private;
		if (x11_window->window == window_id) {
			return window;
		}
	}
	return NULL;
}

static int x11_button2tgui(int x11_button) {
	switch (x11_button) {
	case Button1:
		return TGUI_BUTTON_LEFT;
	case Button2:
		return TGUI_BUTTON_MIDDLE;
	case Button3:
		return TGUI_BUTTON_RIGHT;
	default:
		return 0;
	}
}

static int x11_sym2tgui(int x11_sym) {
	switch (x11_sym) {
	case XK_BackSpace:
		return '\177';
	case XK_Tab:
	case XK_KP_Tab:
		return '\t';
	case XK_Escape:
		return '\033';
	case XK_KP_Equal:
		return '=';
	case XK_KP_Multiply:
		return '*';
	case XK_KP_Add:
		return '+';
	case XK_KP_Separator:
		return '.';
	case XK_KP_Subtract:
		return '-';
	case XK_KP_Divide:
		return '/';
	default:
		return x11_sym;
	}
}

int tgui_platform_init(void) {
	display = XOpenDisplay(NULL);
	if (!display) return -1;

	screen = DefaultScreen(display);
	visual = DefaultVisual(display, screen);
	colormap = DefaultColormap(display, screen);

	return 0;
}
void tgui_platform_fini(void) {
	XCloseDisplay(display);
}

void tgui_platform_handle_event(void) {
	XEvent event;
	XFlush(display);
	XNextEvent(display, &event);
	switch (event.type) {
	case Expose:
		if (event.xexpose.count == 0) {
			tgui_window_t *window = get_window(event.xexpose.window);
			tgui_platform_push_window(window);
		}
		break;
	case ButtonPress:;
		tgui_window_t *window = get_window(event.xbutton.window);
		tgui_input_click(window, x11_button2tgui(event.xbutton.button), event.xbutton.x, event.xbutton.y);
		break;
	case ButtonRelease:;
		window = get_window(event.xbutton.window);
		tgui_input_unclick(window, x11_button2tgui(event.xbutton.button), event.xbutton.x, event.xbutton.y);
		break;
	case MotionNotify:
		window = get_window(event.xmotion.window);
		tgui_input_move(window, event.xmotion.x, event.xmotion.y);
		break;
	case KeyPress:
		window = get_window(event.xkey.window);
		tgui_input_key_press(window, 0, x11_sym2tgui(XLookupKeysym(&event.xkey, 0)));
		break;
	}	
}

int tgui_platform_create_window(tgui_window_t *window) {
	x11_window_t *x11_window = malloc(sizeof(x11_window_t));
	x11_window->window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, window->widget.width, window->widget.height, 1, XBlackPixel(display, screen), XWhitePixel(display, screen));
	XSelectInput(display, x11_window->window, ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | KeyPressMask | KeyReleaseMask);

	// setup pixmap and GC
	XGCValues values;
	x11_window->gc = XCreateGC(display, x11_window->window, 0, &values);
	x11_window->pixmap = XCreatePixmap(display, x11_window->window, window->widget.width, window->widget.height, DefaultDepth(display, screen));
	x11_window->draw = XftDrawCreate(display, x11_window->pixmap, visual, colormap);
	XMapWindow(display, x11_window->window);
	XFlush(display);
	window->private = x11_window;
	return 0;
	
}
void tgui_platform_close_window(tgui_window_t *window) {
	x11_window_t *x11_window = window->private;
	XUnmapWindow(display, x11_window->window);
	XftDrawDestroy(x11_window->draw);
	XFreeGC(display, x11_window->gc);
	XFreePixmap(display, x11_window->pixmap);
	XDestroyWindow(display, x11_window->window);
	free(x11_window);
}

void tgui_platform_push_window(tgui_window_t *window) {
	x11_window_t *x11_window = window->private;
	XCopyArea(display, x11_window->pixmap, x11_window->window, x11_window->gc, 0, 0, window->widget.width, window->widget.height, 0, 0);
}

void tgui_platform_new_color(tgui_color_t *color, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	XRenderColor x11_color = {
		.red    = r * 257,
		.green  = g * 257,
		.blue   = b * 257,
		.alpha  = a * 257,
	};
	color->private = malloc(sizeof(XftColor));
	XftColorAllocValue(display, visual, colormap, &x11_color, color->private);
}

void tgui_platform_free_color(tgui_color_t *color) {
	XftColorFree(display, visual, colormap, color->private);
	free(color->private);
}

int tgui_platform_load_font(tgui_font_t *font) {
	// by default use sans
	const char *name = font->name;
	if (!name) name = "sans-10";

	XftFont *xft_font = XftFontOpenName(display, screen, name);
	if (!xft_font) return -1;
	font->private = xft_font;
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font) {
	XftFontClose(display, font->private);
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	tgui_font_t *font = tgui_widget_get_font(widget);
	XGlyphInfo extents;
	XftTextExtentsUtf8(display, font->private, (const FcChar8*)text, strlen(text), &extents);
	return extents.xOff;
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	(void)text;
	tgui_font_t *font = tgui_widget_get_font(widget);
	XftFont *xft_font = font->private;
	return xft_font->ascent + xft_font->descent;
}

int tgui_platform_load_image(tgui_image_t *image) {
	int width;
	int height;
	int n;
	stbi_uc *data = stbi_load(image->filename, &width, &height, &n, 0);
	if (!data) return -1;
	image->private = data;
	return 0;
}

void tgui_platform_free_image(tgui_image_t *image) {
	stbi_image_free(image->private);
}

void tgui_platform_render_rect(tgui_window_t *window, tgui_color_t *color, long x, long y, long width, long height) {
	printf("render rect %ld %ld %ld %ld\n", x, y, width, height);
	x11_window_t *x11_window = window->private;
	XftColor *xft_color = COLOR2XFT(color);
	XftDrawRect(x11_window->draw, xft_color, x, y, width, height);
}

void tgui_platform_render_rounded_rect(tgui_window_t *window, tgui_color_t *color, long x, long y, long width, long height, char corners, unsigned int rounded_size) {
}

void tgui_platform_render_text(tgui_window_t *window, tgui_widget_t *widget, long x, long y, const char *text) {
	printf("render text %ld %ld %s\n", x, y, text);
	tgui_font_t *font = tgui_widget_get_font(widget);
	XftFont *xft_font = font->private;
	tgui_color_t *color = tgui_widget_get_color(widget);
	x11_window_t *x11_window = window->private;

	XftDrawStringUtf8(x11_window->draw, color->private, xft_font, x, y + xft_font->ascent, (const FcChar8*)text, strlen(text));
}

void tgui_platform_render_image(tgui_window_t *window, long x, long y, tgui_image_t *image) {
	x11_window_t *x11_window = window->private;
}

void tgui_platform_set_clip(tgui_window_t *window, long x, long y, long width, long height) {
	x11_window_t *x11_window = window->private;
	XRectangle rect = {
		.x = x,
		.y = y,
		.width = width,
		.height = height,
	};
	Picture picture = XftDrawPicture(x11_window->draw);
	XRenderSetPictureClipRectangles(display, picture, 0, 0, &rect, 1);
}
