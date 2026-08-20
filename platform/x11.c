#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <platform.h>
#include <timer.h>
#include <inputs.h>
#include <list.h>
#include <poll.h>
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

static tgui_surface_t *get_surface(Window window_id) {
	TGUI_LIST_FOREACH(node, tgui_get_surfaces()) {
		tgui_surface_t *surface = TGUI_CONTAINER_OF(node, tgui_surface_t, node);
		x11_window_t *x11_window = surface->private;
		if (x11_window->window == window_id) {
			return surface;
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
	case XK_Return:
		return TGUI_KEY_ENTER;
	case XK_Up:
		return TGUI_KEY_ARROW_UP;
	case XK_Down:
		return TGUI_KEY_ARROW_DOWN;
	case XK_Left:
		return TGUI_KEY_ARROW_LEFT;
	case XK_Right:
		return TGUI_KEY_ARROW_RIGHT;
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
	struct pollfd pollfd = {
		.fd = tgui_platform_get_fd(),
		.events = POLLIN,
	};
	if (XPending(display) <= 0) {
		poll(&pollfd, 1, tgui_timer_get_before_next_trigger());
	}
	if (XPending(display) <= 0) return;
	XNextEvent(display, &event);
	switch (event.type) {
	case Expose:
		if (event.xexpose.count == 0) {
			tgui_surface_t *surface = get_surface(event.xexpose.window);
			tgui_platform_push_surface(surface);
		}
		break;
	case ButtonPress:;
		tgui_surface_t *surface = get_surface(event.xbutton.window);
		tgui_input_click(surface, x11_button2tgui(event.xbutton.button), event.xbutton.x, event.xbutton.y);
		break;
	case ButtonRelease:;
		surface = get_surface(event.xbutton.window);
		tgui_input_unclick(surface, x11_button2tgui(event.xbutton.button), event.xbutton.x, event.xbutton.y);
		break;
	case MotionNotify:
		surface = get_surface(event.xmotion.window);
		tgui_input_move(surface, event.xmotion.x, event.xmotion.y);
		break;
	case FocusIn:
		surface = get_surface(event.xmotion.window);
		tgui_input_focus(surface);
		break;
	case FocusOut:
		surface = get_surface(event.xmotion.window);
		tgui_input_unfocus(surface);
		break;
	case KeyPress:
		surface = get_surface(event.xkey.window);
		tgui_input_key_press(surface, 0, x11_sym2tgui(XLookupKeysym(&event.xkey, 0)));
		break;
	case KeyRelease:
		surface = get_surface(event.xkey.window);
		tgui_input_key_release(surface, 0, x11_sym2tgui(XLookupKeysym(&event.xkey, 0)));
		break;
	}	
}

int tgui_platform_create_window(tgui_window_t *window) {
	return tgui_platform_create_surface(&window->surface, NULL);
}

void tgui_platform_close_window(tgui_window_t *window) {
	tgui_platform_close_surface(&window->surface);
}

void tgui_platform_minimize_window(tgui_window_t *window) {
}

void tgui_platform_toggle_maximize_window(tgui_window_t *window) {
}

int tgui_platform_create_surface(tgui_surface_t *surface, tgui_surface_t *parent) {
	Window x11_parent;
	if (parent) {
		x11_window_t *x11_window = parent->private;
		x11_parent = x11_window->window;
	} else {
		x11_parent = RootWindow(display, screen);
	}
	x11_window_t *x11_window = malloc(sizeof(x11_window_t));
	x11_window->window = XCreateSimpleWindow(display, x11_parent, 0, 0, surface->width, surface->height, 1, XBlackPixel(display, screen), XWhitePixel(display, screen));
	XSelectInput(display, x11_window->window, ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | FocusChangeMask);

	// setup pixmap and GC
	XGCValues values;
	x11_window->gc = XCreateGC(display, x11_window->window, 0, &values);
	x11_window->pixmap = XCreatePixmap(display, x11_window->window, surface->width, surface->height, DefaultDepth(display, screen));
	x11_window->draw = XftDrawCreate(display, x11_window->pixmap, visual, colormap);
	XMapWindow(display, x11_window->window);
	XFlush(display);
	surface->private = x11_window;
	return 0;
}

void tgui_platform_close_surface(tgui_surface_t *surface) {
	x11_window_t *x11_window = surface->private;
	XUnmapWindow(display, x11_window->window);
	XftDrawDestroy(x11_window->draw);
	XFreeGC(display, x11_window->gc);
	XFreePixmap(display, x11_window->pixmap);
	XDestroyWindow(display, x11_window->window);
	free(x11_window);
}

void tgui_platform_push_surface(tgui_surface_t *surface) {
	x11_window_t *x11_window = surface->private;
	XCopyArea(display, x11_window->pixmap, x11_window->window, x11_window->gc, 0, 0, surface->width, surface->height, 0, 0);
}

void tgui_platform_set_surface_visible(tgui_surface_t *surface, int visible) {
	x11_window_t *x11_window = surface->private;
	if (visible) {
		XMapWindow(display, x11_window->window);
	} else {
		XUnmapWindow(display, x11_window->window);
	}
}

void tgui_platform_set_surface_position(tgui_surface_t *surface, long x, long y) {
	x11_window_t *x11_window = surface->private;
	XMoveWindow(display, x11_window->window, x, y);
}

void tgui_platform_grab_surface(tgui_surface_t *surface) {
	x11_window_t *x11_window = surface->private;
	XGrabPointer(display, x11_window->window, False, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	XGrabKeyboard(display, x11_window->window, True, GrabModeAsync, GrabModeAsync,  CurrentTime);
}

void tgui_platform_ungrab_surface(tgui_surface_t *surface) {
	(void)surface;
	XUngrabPointer(display, CurrentTime);
	XUngrabKeyboard(display, CurrentTime);
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

int tgui_platform_load_font(tgui_font_t *font, tgui_sized_font_t *sized) {
	if (!sized) return 0;
	
	// by default use sans
	const char *name = font->name;
	if (!name) name = "sans";

	char font_name[64];
	sprintf(font_name, "%s-%u", name, sized->size);

	XftFont *xft_font = XftFontOpenName(display, screen, font_name);
	if (!xft_font) return -1;
	sized->private = xft_font;
	return 0;
}

void tgui_platform_free_font(tgui_font_t *font, tgui_sized_font_t *sized) {
	(void)font;
	if (!sized) return;
	XftFontClose(display, sized->private);
}

int tgui_platform_text_width(tgui_widget_t *widget, const char *text) {
	tgui_font_t *font = tgui_widget_get_font(widget);
	tgui_sized_font_t *sized_font = tgui_font_get_sized(font, tgui_widget_get_font_size(widget));
	XftFont *xft_font = sized_font->private;
	XGlyphInfo extents;
	XftTextExtentsUtf8(display, xft_font, (const FcChar8*)text, strlen(text), &extents);
	return extents.xOff;
}

int tgui_platform_text_height(tgui_widget_t *widget, const char *text) {
	(void)text;
	tgui_font_t *font = tgui_widget_get_font(widget);
	tgui_sized_font_t *sized_font = tgui_font_get_sized(font, tgui_widget_get_font_size(widget));
	XftFont *xft_font = sized_font->private;
	return xft_font->ascent + xft_font->descent;
}

int tgui_platform_load_image(tgui_image_t *image) {
	image->width = 16;
	image->height = 16;
	return 0;
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

void tgui_platform_render_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height) {
	printf("render rect %ld %ld %ld %ld\n", x, y, width, height);
	x11_window_t *x11_window = surface->private;
	XftColor *xft_color = COLOR2XFT(color);
	XftDrawRect(x11_window->draw, xft_color, x, y, width, height);
}

static Picture create_gradient(long rayon, XRenderColor color) {
	XFixed f_rayon = XDoubleToFixed(rayon);
	XRadialGradient grad = {
		.inner = {f_rayon, f_rayon, 0},
		.outer = {f_rayon, f_rayon, f_rayon},
	};
	XFixed stops[] = {
		XDoubleToFixed(0),
		XDoubleToFixed(0.90f),
		XDoubleToFixed(1)
	};
	XRenderColor colors[] = {
		color,
		color,
		{0, 0, 0, 0},
	};

	return XRenderCreateRadialGradient(display, &grad, stops, colors, 3);
}

static Picture create_gradient_outline(long rayon, XRenderColor color, unsigned int border_size) {
	XFixed f_rayon = XDoubleToFixed(rayon);
	XRadialGradient grad = {
		.inner = {f_rayon, f_rayon, 0},
		.outer = {f_rayon, f_rayon, f_rayon},
	};
	XFixed stops[] = {
		//XDoubleToFixed(0),
		XDoubleToFixed((rayon - (double)border_size-0.1f)/rayon),
		XDoubleToFixed((rayon - (double)border_size)/(double)rayon),
		XDoubleToFixed(((double)rayon-0.1f)/(double)rayon),
		XDoubleToFixed(1)
	};
	XRenderColor colors[] = {
		{0, 0, 0, 0},
		color,
		color,
		{0, 0, 0, 0},
	};

	return XRenderCreateRadialGradient(display, &grad, stops, colors, 4);
}

static void render_corner(Picture gradient, x11_window_t *window, unsigned int rayon, int round, long src_x, long src_y, long x, long y) {
	if (round) {
		XRenderComposite(display, PictOpOver, gradient, None, XftDrawPicture(window->draw), src_x, src_y, 0, 0, x, y, rayon, rayon);
	}
}

void tgui_platform_render_rounded_rect(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, char corners, unsigned int rayon) {
	x11_window_t *x11_window = surface->private;
	XftColor *xft_color = COLOR2XFT(color);
	XftDrawRect(x11_window->draw, xft_color, x, y + rayon, width, height - 2 * rayon);
	XftDrawRect(x11_window->draw, xft_color, x + rayon, y, width - 2 * rayon, height);

	// render corners
	Picture gradient = create_gradient(rayon, xft_color->color);

	render_corner(gradient, x11_window, rayon, corners & TGUI_CORNER_TOP_LEFT, 0, 0, x, y);
	render_corner(gradient, x11_window, rayon, corners & TGUI_CORNER_TOP_RIGHT, rayon, 0, x + width - rayon, y);
	render_corner(gradient, x11_window, rayon, corners & TGUI_CORNER_BOTTOM_LEFT, 0, rayon, x, y + height - rayon);
	render_corner(gradient, x11_window, rayon, corners & TGUI_CORNER_BOTTOM_RIGHT, rayon, rayon, x + width - rayon, y + height - rayon);

	XRenderFreePicture(display, gradient);
}

void tgui_platform_render_rounded_rect_outline(tgui_surface_t *surface, tgui_color_t *color, long x, long y, long width, long height, unsigned int border_size, unsigned int rayon) {
	x11_window_t *x11_window = surface->private;
	XftColor *xft_color = COLOR2XFT(color);
	XftDrawRect(x11_window->draw, xft_color, x + rayon, y, width - 2 * rayon, border_size);
	XftDrawRect(x11_window->draw, xft_color, x + rayon, y + height - border_size, width - 2 * rayon, border_size);
	XftDrawRect(x11_window->draw, xft_color, x, y + rayon, border_size, height - 2 * rayon);
	XftDrawRect(x11_window->draw, xft_color, x + width - border_size, y + rayon, border_size, height - 2 * rayon);

	// render corners
	Picture gradient = create_gradient_outline(rayon, xft_color->color, border_size);

	render_corner(gradient, x11_window, rayon, 1, 0, 0, x, y);
	render_corner(gradient, x11_window, rayon, 1, rayon, 0, x + width - rayon, y);
	render_corner(gradient, x11_window, rayon, 1, 0, rayon, x, y + height - rayon);
	render_corner(gradient, x11_window, rayon, 1, rayon, rayon, x + width - rayon, y + height - rayon);

	XRenderFreePicture(display, gradient);
}

void tgui_platform_render_text(tgui_surface_t *surface, tgui_widget_t *widget, long x, long y, const char *text) {
	printf("render text %ld %ld %s\n", x, y, text);
	tgui_font_t *font = tgui_widget_get_font(widget);
	tgui_sized_font_t *sized_font = tgui_font_get_sized(font, tgui_widget_get_font_size(widget));
	XftFont *xft_font = sized_font->private;
	tgui_color_t *color = tgui_widget_get_color(widget);
	x11_window_t *x11_window = surface->private;

	XftDrawStringUtf8(x11_window->draw, color->private, xft_font, x, y + xft_font->ascent, (const FcChar8*)text, strlen(text));
}

void tgui_platform_render_image(tgui_surface_t *surface, long x, long y, tgui_image_t *image) {
	x11_window_t *x11_window = surface->private;
}

void tgui_platform_set_clip(tgui_surface_t *surface, long x, long y, long width, long height) {
	x11_window_t *x11_window = surface->private;
	XRectangle rect = {
		.x = x,
		.y = y,
		.width = width,
		.height = height,
	};
	Picture picture = XftDrawPicture(x11_window->draw);
	XRenderSetPictureClipRectangles(display, picture, 0, 0, &rect, 1);
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
	return XConnectionNumber(display);
}

void tgui_platform_register_handler(int (*handler)(void *event, void *data), void *data) {
}
