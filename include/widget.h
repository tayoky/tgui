#ifndef TGUI_WIDGET_H
#define TGUI_WIDGET_H

#include <stddef.h>
#include "list.h"
#include "color.h"
#include "font.h"
#include "style.h"

typedef struct tgui_widget tgui_widget_t;

typedef struct tgui_widget_class {
	size_t size;
	const char *name;
	tgui_widget_t *(*new)(void);
	void (*free)(tgui_widget_t *);
	void (*calculate_sizes)(tgui_widget_t *);
	void (*allocate_space)(tgui_widget_t *);
	void (*remove_child)(tgui_widget_t *, tgui_widget_t *child);
	void (*render)(tgui_widget_t *);
} tgui_widget_class_t;

#define TGUI_STATE_NORMAL   0
#define TGUI_STATE_HOVER    1
#define TGUI_STATE_CLICK    2
#define TGUI_STATE_DISABLED 3
#define TGUI_STATE_COUNT    4

typedef struct tgui_style_node {
	tgui_list_node_t node;
	tgui_style_t *style;
} tgui_style_node_t;

#define TGUI_STYLE_FROM_NODE(n) (TGUI_CONTAINER_OF(n, tgui_style_node_t, node)->style)

struct tgui_widget {
	tgui_list_node_t node;
	tgui_list_t children;
	tgui_widget_t *parent;
	tgui_widget_class_t *class;
	tgui_list_t state_styles[TGUI_STATE_COUNT];
	tgui_list_t styles;
	long flags;
	long left_margin;
	long right_margin;
	long bottom_margin;
	long top_margin;
	long left_padding;
	long right_padding;
	long bottom_padding;
	long top_padding;
	long width;
	long height;
	long x;
	long y;
	long min_width;
	long min_height;
	long pref_width;
	long pref_height;
	char valign;
	char halign;
	char state;
};

#define TGUI_WIDGET_HEXPAND    0x01
#define TGUI_WIDGET_VEXPAND    0x02
#define TGUI_WIDGET_DIRTY      0x04
#define TGUI_WIDGET_DIRTY_SIZE 0x04

#define TGUI_ALIGN_FILL    0x00
#define TGUI_ALIGN_LEFT    0x01
#define TGUI_ALIGN_TOP     0x01
#define TGUI_ALIGN_CENTER  0x02
#define TGUI_ALIGN_RIGHT   0x03
#define TGUI_ALIGN_BOTTOM  0x03

#define TGUI_WIDGET_CAST(w) (&(w)->widget)
#define TGUI_CONTAINER_OF(ptr, type, member) ((type *)((char*)(ptr) - offsetof(type, member)))
#define TGUI_WIDGET_FROM_NODE(n) TGUI_CONTAINER_OF(n, tgui_widget_t, node)

tgui_widget_t *tgui_widget_new(tgui_widget_class_t *class);
void tgui_widget_destroy(tgui_widget_t *widget);
void tgui_widget_calculate_sizes(tgui_widget_t *widget);
void tgui_widget_allocate_space(tgui_widget_t *widget, long x, long y, long width, long height);
void tgui_widget_render(tgui_widget_t *widget);
int tgui_widget_is_class(tgui_widget_t *widget, const char *class_name);

static inline void tgui_widget_mark_dirty(tgui_widget_t *widget) {
	while (widget) {
		widget->flags |= TGUI_WIDGET_DIRTY;
		widget = widget->parent;
	}
}

static inline int tgui_widget_is_dirty(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_DIRTY;
}

static inline void tgui_widget_mark_dirty_size(tgui_widget_t *widget) {
	while (widget) {
		widget->flags |= TGUI_WIDGET_DIRTY_SIZE | TGUI_WIDGET_DIRTY;
		widget = widget->parent;
	}
}

static inline int tgui_widget_is_dirty_size(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_DIRTY_SIZE;
}

static inline void tgui_widget_set_state(tgui_widget_t *widget, char state) {
	if (widget->state == state) return;
	widget->state = state;
	tgui_widget_mark_dirty(widget);
}

static inline char tgui_widget_get_state(tgui_widget_t *widget) {
	return widget->state;
}

void tgui_widget_add_state_style(tgui_widget_t *widget, char state, tgui_style_t *style);
void tgui_widget_add_style(tgui_widget_t *widget, tgui_style_t *style);
tgui_list_t *tgui_widget_get_styles(tgui_widget_t *widget);
void tgui_widget_get_current_style(tgui_widget_t *widget, tgui_style_t *style);

unsigned int tgui_widget_get_border_width(tgui_widget_t *widget, int side);
tgui_color_t *tgui_widget_get_border_color(tgui_style_t *style, int side);
char tgui_widget_get_border_style(tgui_widget_t *widget, int side);
tgui_color_t *tgui_widget_get_color(tgui_widget_t *widget);
tgui_color_t *tgui_widget_get_background_color(tgui_widget_t *widget);
tgui_font_t *tgui_widget_get_font(tgui_widget_t *widget);
unsigned int tgui_widget_get_font_size(tgui_widget_t *widget);

/*
static inline tgui_font_t *tgui_widget_get_font(tgui_widget_t *widget) {
	while (widget) {
		tgui_font_t *font = tgui_style_get_font(tgui_widget_get_style(widget));
		if (font) {
			return font;
		}
		widget = widget->parent;
	}
	return tgui_font_get_default();
}

static inline void tgui_widget_set_color(tgui_widget_t *widget, tgui_color_t *color) {
	tgui_style_set_color(tgui_widget_get_style(widget), color);
}

static inline tgui_color_t *tgui_widget_get_color(tgui_widget_t *widget) {
	while (widget) {
		tgui_color_t *color = tgui_style_get_color(tgui_widget_get_style(widget));
		if (color) {
			return color;
		}
		widget = widget->parent;
	}
	return NULL;
}

static inline void tgui_widget_set_background_color(tgui_widget_t *widget, tgui_color_t *color) {
	tgui_style_set_background_color(tgui_widget_get_style(widget), color);
}

static inline tgui_color_t *tgui_widget_get_background_color(tgui_widget_t *widget) {
	return tgui_style_get_background_color(tgui_widget_get_style(widget));
}*/

static inline void tgui_widget_set_left_margin(tgui_widget_t *widget, long margin) {
	widget->left_margin = margin;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_right_margin(tgui_widget_t *widget, long margin) {
	widget->right_margin = margin;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_top_margin(tgui_widget_t *widget, long margin) {
	widget->top_margin = margin;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_bottom_margin(tgui_widget_t *widget, long margin) {
	widget->bottom_margin = margin;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_margin(tgui_widget_t *widget, long margin) {
	widget->left_margin = margin;
	widget->right_margin = margin;
	widget->top_margin = margin;
	widget->bottom_margin = margin;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_left_padding(tgui_widget_t *widget, long padding) {
	widget->left_padding = padding;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_right_padding(tgui_widget_t *widget, long padding) {
	widget->right_padding = padding;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_top_padding(tgui_widget_t *widget, long padding) {
	widget->top_padding = padding;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_bottom_padding(tgui_widget_t *widget, long padding) {
	widget->bottom_padding = padding;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_padding(tgui_widget_t *widget, long padding) {
	widget->left_padding = padding;
	widget->right_padding = padding;
	widget->top_padding = padding;
	widget->bottom_padding = padding;
	tgui_widget_mark_dirty_size(widget);
}

static inline void tgui_widget_set_halign(tgui_widget_t *widget, char halign) {
	widget->halign = halign;
	tgui_widget_mark_dirty_size(widget);
}

static inline char tgui_widget_get_halign(tgui_widget_t *widget) {
	return widget->halign;
}

static inline void tgui_widget_set_valign(tgui_widget_t *widget, char valign) {
	widget->valign = valign;
	tgui_widget_mark_dirty_size(widget);
}

static inline char tgui_widget_get_valign(tgui_widget_t *widget) {
	return widget->valign;
}

static inline long tgui_widget_get_outer_x(tgui_widget_t *widget) {
	return widget->x;
}

static inline long tgui_widget_get_outer_width(tgui_widget_t *widget) {
	return widget->width;
}

static inline long tgui_widget_get_outer_y(tgui_widget_t *widget) {
	return widget->y;
}

static inline long tgui_widget_get_outer_height(tgui_widget_t *widget) {
	return widget->height;
}

static inline long tgui_widget_get_frame_x(tgui_widget_t *widget) {
	return widget->x + widget->left_margin;
}

static inline long tgui_widget_get_frame_width(tgui_widget_t *widget) {
	return widget->width - widget->left_margin - widget->right_margin;
}

static inline long tgui_widget_get_frame_y(tgui_widget_t *widget) {
	return widget->y + widget->top_margin;
}

static inline long tgui_widget_get_frame_height(tgui_widget_t *widget) {
	return widget->height - widget->top_margin - widget->bottom_margin;
}

static inline long tgui_widget_get_inner_x(tgui_widget_t *widget) {
	return tgui_widget_get_frame_x(widget) + widget->left_padding;
}

static inline long tgui_widget_get_inner_width(tgui_widget_t *widget) {
	return tgui_widget_get_frame_width(widget) - widget->left_padding - widget->right_padding;
}

static inline long tgui_widget_get_inner_y(tgui_widget_t *widget) {
	return tgui_widget_get_frame_y(widget) + widget->top_padding;
}

static inline long tgui_widget_get_inner_height(tgui_widget_t *widget) {
	return tgui_widget_get_frame_height(widget) - widget->top_padding - widget->bottom_padding;
}

void tgui_widget_set_parent(tgui_widget_t *child, tgui_widget_t *parent);
void tgui_widget_remove_parent(tgui_widget_t *child);

#endif
