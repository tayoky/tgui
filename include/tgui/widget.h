#ifndef TGUI_WIDGET_H
#define TGUI_WIDGET_H

#include <stddef.h>
#include "list.h"
#include "color.h"
#include "font.h"
#include "style.h"
#include "events.h"
#include "tobject.h"
#include "rect.h"

TOBJECT_DECLARE_CLASS(tgui_widget, TGUI_WIDGET)

struct tgui_widget_class {
	tobject_class_t parent_class;
	void (*calculate_sizes)(tgui_widget_t *);
	void (*allocate_space)(tgui_widget_t *);
	void (*remove_child)(tgui_widget_t *, tgui_widget_t *child);
	void (*render)(tgui_widget_t *);
	void (*after_render)(tgui_widget_t *);
	void (*set_orientation)(tgui_widget_t *, int);
};

#define TGUI_STATE_NORMAL   0
#define TGUI_STATE_HOVER    1
#define TGUI_STATE_PRESSED  2
#define TGUI_STATE_DISABLED 3
#define TGUI_STATE_FOCUSED  4
#define TGUI_STATE_COUNT    5

typedef struct tgui_style_node {
	tgui_list_node_t node;
	tgui_style_t *style;
} tgui_style_node_t;

#define TGUI_STYLE_FROM_NODE(n) (TGUI_CONTAINER_OF(n, tgui_style_node_t, node)->style)

struct tgui_widget {
	tobject_t tobject;
	tgui_list_node_t node;
	tgui_list_t children;
	tgui_widget_t *parent;
	char *id;
	void *layout_data; // usef by parent
	tgui_list_t state_styles[TGUI_STATE_COUNT];
	tgui_list_t styles;
	tgui_style_t cache_style;
	long flags;
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
	char state_flags;
	char orientation;
};

#define TGUI_WIDGET_HEXPAND     0x01
#define TGUI_WIDGET_VEXPAND     0x02
#define TGUI_WIDGET_DIRTY       0x04
#define TGUI_WIDGET_DIRTY_SIZE  0x08
#define TGUI_WIDGET_DIRTY_STYLE 0x10
#define TGUI_WIDGET_DIRTY_SPACE 0x20
#define TGUI_WIDGET_HIDDEN      0x40
#define TGUI_WIDGET_SELECTABLE  0x80

#define TGUI_ALIGN_FILL    0x00
#define TGUI_ALIGN_LEFT    0x01
#define TGUI_ALIGN_TOP     0x01
#define TGUI_ALIGN_CENTER  0x02
#define TGUI_ALIGN_RIGHT   0x03
#define TGUI_ALIGN_BOTTOM  0x03

#define TGUI_ORIENTATION_VERTICAL   0
#define TGUI_ORIENTATION_HORIZONTAL 1

#define TGUI_WIDGET_FROM_NODE(n) TGUI_CONTAINER_OF(n, tgui_widget_t, node)

#define TGUI_FALSE 0
#define TGUI_TRUE  1

void tgui_widget_destroy(tgui_widget_t *widget);
void tgui_widget_calculate_sizes(tgui_widget_t *widget);
void tgui_widget_allocate_space(tgui_widget_t *widget, long x, long y, long width, long height);
void tgui_widget_render(tgui_widget_t *widget);
void tgui_widget_set_id(tgui_widget_t *widget, const char *id);

void tgui_widget_mark_dirty(tgui_widget_t *widget);

static inline void tgui_widget_mark_dirty_size(tgui_widget_t *widget) {
	while (widget) {
		widget->flags |= TGUI_WIDGET_DIRTY_SIZE;
		widget = widget->parent;
	}
}

static inline int tgui_widget_is_dirty_size(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_DIRTY_SIZE;
}

static inline void tgui_widget_mark_dirty_space(tgui_widget_t *widget) {
	while (widget) {
		widget->flags |= TGUI_WIDGET_DIRTY_SPACE;
		widget = widget->parent;
	}
}

static inline int tgui_widget_is_dirty_space(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_DIRTY_SPACE;
}

static inline void tgui_widget_mark_dirty_style(tgui_widget_t *widget) {
	if (!widget) return;
	widget->flags |= TGUI_WIDGET_DIRTY_STYLE;
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_size(widget->parent);
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		tgui_widget_mark_dirty_style(child);
	}
}

static inline int tgui_widget_is_dirty_style(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_DIRTY_STYLE;
}

static inline void tgui_widget_hide(tgui_widget_t *widget) {
	if (!widget) return;
	if (widget->flags & TGUI_WIDGET_HIDDEN) return;
	widget->flags |= TGUI_WIDGET_HIDDEN;
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_space(widget->parent);
	tgui_widget_mark_dirty_size(widget->parent);
}

static inline void tgui_widget_show(tgui_widget_t *widget) {
	if (!widget) return;
	if (!(widget->flags & TGUI_WIDGET_HIDDEN)) return;
	widget->flags &= ~TGUI_WIDGET_HIDDEN;
	widget->width  = widget->x = 0;
	widget->height = widget->y = 0;
	tgui_widget_mark_dirty_space(widget);
	tgui_widget_mark_dirty_size(widget->parent);
}

/**
 * @brief set the visibility of a widget
 * @param widget the widget to set the visibility of
 * @param visible is the widget visible
 */
static inline void tgui_widget_set_visible(tgui_widget_t *widget, int visible) {
	if (!widget) return;
	if (visible) {
		tgui_widget_show(widget);
	} else {
		tgui_widget_hide(widget);
	}
}

static inline int tgui_widget_is_hidden(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_HIDDEN;
}

static inline void tgui_widget_set_selectable(tgui_widget_t *widget, int selectable) {
	if (!widget) return;
	if (selectable) {
		widget->flags |= TGUI_WIDGET_SELECTABLE;
	} else {
		widget->flags &= ~TGUI_WIDGET_SELECTABLE;
	}
}

/**
 * @brief check if a widget is selectable
 * @param widget the widget to check
 * @return 1 if selectable else 0
 */
static inline int tgui_widget_is_selectable(tgui_widget_t *widget) {
	return widget->flags & TGUI_WIDGET_SELECTABLE;
}

/**
 * @brief send a signal to a widget and all of it's parent
 * @param widget the widget to send the signal to
 * @param signal the signal to send
 * @param event event data for the signal (can be NULL)
 */
void tgui_widget_send_parent_signal(tgui_widget_t *widget, const char *signal, void *event);

static inline void tgui_widget_set_hexpand(tgui_widget_t *widget, int hexpand) {
	if (hexpand) {
		widget->flags |= TGUI_WIDGET_HEXPAND;
	} else {
		widget->flags &= ~TGUI_WIDGET_HEXPAND;
	}
	tgui_widget_mark_dirty_size(widget->parent);
}

static inline void tgui_widget_set_vexpand(tgui_widget_t *widget, int hexpand) {
	if (hexpand) {
		widget->flags |= TGUI_WIDGET_VEXPAND;
	} else {
		widget->flags &= ~TGUI_WIDGET_VEXPAND;
	}
	tgui_widget_mark_dirty_size(widget->parent);
}

/**
 * @brief set the state of a widget
 * @param widget the widget to set the state of
 * @param state the new state of the widget
 */
static inline void tgui_widget_set_state(tgui_widget_t *widget, char state, int enabled) {
	if (!widget) return;
	char state_flags = widget->state_flags;
	if (enabled) {
		state_flags |= 1 << state;
	} else {
		state_flags &= ~(1 << state);
	}
	if (widget->state_flags == state_flags) return;
	if (widget->state_styles[(int)state].first) {
		tgui_widget_mark_dirty(widget);
		widget->state_flags = state_flags;
		tgui_widget_mark_dirty_style(widget);
	} else {
		widget->state_flags = state_flags;
	}
}

/**
 * @brief set the state of a widget and its parents
 * @param widget the widget to set the state of
 * @param state the state to set
 * @param enabled is the state enabled
 */
static inline void tgui_widget_set_state_parent(tgui_widget_t *widget, char state, int enabled) {
	while (widget) {
		tgui_widget_set_state(widget, state, enabled);
		widget = widget->parent;
	}
}

/**
 * @brief set the state of a widget and its parents but only until a specified widget
 * @param widget the widget to set the state of
 * @param stop the widget to stop at (the state of thiw widget and its parent will not be changed)
 * @param state the state to set
 * @param enabled is the new state enabled
 * @return 1 if stoped because of stop else return 0
 */
static inline int tgui_widget_set_state_parent_stop(tgui_widget_t *widget, tgui_widget_t *stop, char state, int enabled) {
	while (widget && widget != stop) {
		tgui_widget_set_state(widget, state, enabled);
		widget = widget->parent;
	}
	return widget == stop;
}

/**
 * @brief get the state of a widget
 * @param widget the widget to get the state of
 * @param state the state to check
 * @return 1 if the specified state is active else 0
 */
static inline int tgui_widget_get_state(tgui_widget_t *widget, char state) {
	return widget->state_flags & (1 << state);
}

static inline int tgui_widget_is_parent(tgui_widget_t *parent, tgui_widget_t *child) {
	while (child) {
		if (child == parent) return 1;
		child = child->parent;
	}
	return 0;
}

static inline void tgui_widget_set_orientation(tgui_widget_t *widget, char orientation) {
	if (widget->orientation == orientation) return;
	widget->orientation = orientation;
	tgui_widget_class_t *class = tgui_widget_get_class(widget);
	if (class->set_orientation) {
		class->set_orientation(widget, orientation);
	}
	tgui_widget_mark_dirty_size(widget);
}

static inline char tgui_widget_get_orientation(tgui_widget_t *widget) {
	return widget->orientation;
}

tgui_widget_t *tgui_widget_get_at(tgui_widget_t *parent, long x, long y);

void tgui_widget_set_class_style(tgui_style_t *style, const char *class);
void tgui_widget_set_class_state_style(tgui_style_t *style, const char *class, char state);
void tgui_widget_apply_class_styles(tgui_widget_t *widget, const char *name);
void tgui_widget_apply_default_styles(tgui_widget_t *widget);
void tgui_widget_add_state_style(tgui_widget_t *widget, char state, tgui_style_t *style);
void tgui_widget_add_style(tgui_widget_t *widget, tgui_style_t *style);
void tgui_widget_remove_state_style(tgui_widget_t *widget, char state, tgui_style_t *style);
void tgui_widget_remove_style(tgui_widget_t *widget, tgui_style_t *style);
tgui_list_t *tgui_widget_get_state_styles(tgui_widget_t *widget, char state);
tgui_list_t *tgui_widget_get_styles(tgui_widget_t *widget);
tgui_style_t *tgui_widget_get_current_style(tgui_widget_t *widget);

unsigned int tgui_widget_get_border_size(tgui_widget_t *widget, int side);
tgui_color_t *tgui_widget_get_border_color(tgui_style_t *style, int side);
char tgui_widget_get_border_style(tgui_widget_t *widget, int side);

static inline unsigned int tgui_widget_get_padding(tgui_widget_t *widget, int side) {
	return tgui_widget_get_current_style(widget)->padding[side];
}

static inline unsigned int tgui_widget_get_margin(tgui_widget_t *widget, int side) {
	return tgui_widget_get_current_style(widget)->margin[side];
}

tgui_color_t *tgui_widget_get_color(tgui_widget_t *widget);
tgui_color_t *tgui_widget_get_background_color(tgui_widget_t *widget);
tgui_font_t *tgui_widget_get_font(tgui_widget_t *widget);
unsigned int tgui_widget_get_font_size(tgui_widget_t *widget);

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

static inline void tgui_widget_get_outer_bounds(tgui_widget_t *widget, tgui_rect_t *rect) {
	long x = tgui_widget_get_outer_x(widget);
	long y = tgui_widget_get_outer_y(widget);
	long width  = tgui_widget_get_outer_width(widget);
	long height = tgui_widget_get_outer_height(widget);
	tgui_rect_init(rect, x, y, width, height);
}

static inline long tgui_widget_get_border_x(tgui_widget_t *widget) {
	return widget->x + tgui_widget_get_margin(widget, TGUI_SIDE_LEFT);
}

static inline long tgui_widget_get_border_width(tgui_widget_t *widget) {
	return widget->width - tgui_widget_get_margin(widget, TGUI_SIDE_LEFT) - tgui_widget_get_margin(widget, TGUI_SIDE_RIGHT);
}

static inline long tgui_widget_get_border_y(tgui_widget_t *widget) {
	return widget->y + tgui_widget_get_margin(widget, TGUI_SIDE_TOP);
}

static inline long tgui_widget_get_border_height(tgui_widget_t *widget) {
	return widget->height - tgui_widget_get_margin(widget, TGUI_SIDE_TOP) - tgui_widget_get_margin(widget, TGUI_SIDE_BOTTOM);
}

static inline void tgui_widget_get_border_bounds(tgui_widget_t *widget, tgui_rect_t *rect) {
	long x = tgui_widget_get_border_x(widget);
	long y = tgui_widget_get_border_y(widget);
	long width  = tgui_widget_get_border_width(widget);
	long height = tgui_widget_get_border_height(widget);
	tgui_rect_init(rect, x, y, width, height);
}

static inline long tgui_widget_get_frame_x(tgui_widget_t *widget) {
	tgui_style_t *style = tgui_widget_get_current_style(widget);
	long x = widget->x + tgui_style_get_margin(style, TGUI_SIDE_LEFT);
	if (style->border_style[TGUI_SIDE_LEFT] != TGUI_BORDER_NONE) {
		x += style->border_width[TGUI_SIDE_LEFT];
	}
	return x;
}

static inline long tgui_widget_get_frame_width(tgui_widget_t *widget) {
	tgui_style_t *style = tgui_widget_get_current_style(widget);
	long width = widget->width - tgui_style_get_margin(style, TGUI_SIDE_LEFT) - tgui_style_get_margin(style, TGUI_SIDE_RIGHT);
	if (style->border_style[TGUI_SIDE_LEFT] != TGUI_BORDER_NONE) {
		width -= style->border_width[TGUI_SIDE_LEFT];
	}
	if (style->border_style[TGUI_SIDE_RIGHT] != TGUI_BORDER_NONE) {
		width -= style->border_width[TGUI_SIDE_RIGHT];
	}
	return width;
}

static inline long tgui_widget_get_frame_y(tgui_widget_t *widget) {
	tgui_style_t *style = tgui_widget_get_current_style(widget);
	long y = widget->y + tgui_style_get_margin(style, TGUI_SIDE_TOP);
	if (style->border_style[TGUI_SIDE_TOP] != TGUI_BORDER_NONE) {
		y += style->border_width[TGUI_SIDE_TOP];
	}
	return y;
}

static inline long tgui_widget_get_frame_height(tgui_widget_t *widget) {
	tgui_style_t *style = tgui_widget_get_current_style(widget);
	long height = widget->height - tgui_style_get_margin(style, TGUI_SIDE_TOP) - tgui_style_get_margin(style, TGUI_SIDE_BOTTOM);
	if (style->border_style[TGUI_SIDE_TOP] != TGUI_BORDER_NONE) {
		height -= style->border_width[TGUI_SIDE_TOP];
	}
	if (style->border_style[TGUI_SIDE_BOTTOM] != TGUI_BORDER_NONE) {
		height -= style->border_width[TGUI_SIDE_BOTTOM];
	}
	return height;
}

static inline void tgui_widget_get_frame_bounds(tgui_widget_t *widget, tgui_rect_t *rect) {
	long x = tgui_widget_get_frame_x(widget);
	long y = tgui_widget_get_frame_y(widget);
	long width  = tgui_widget_get_frame_width(widget);
	long height = tgui_widget_get_frame_height(widget);
	tgui_rect_init(rect, x, y, width, height);
}

static inline long tgui_widget_get_inner_x(tgui_widget_t *widget) {
	return tgui_widget_get_frame_x(widget) + tgui_widget_get_padding(widget, TGUI_SIDE_LEFT);
}

static inline long tgui_widget_get_inner_width(tgui_widget_t *widget) {
	return tgui_widget_get_frame_width(widget) - tgui_widget_get_padding(widget, TGUI_SIDE_LEFT) - tgui_widget_get_padding(widget, TGUI_SIDE_RIGHT);
}

static inline long tgui_widget_get_inner_y(tgui_widget_t *widget) {
	return tgui_widget_get_frame_y(widget) + tgui_widget_get_padding(widget, TGUI_SIDE_TOP);
}

static inline long tgui_widget_get_inner_height(tgui_widget_t *widget) {
	return tgui_widget_get_frame_height(widget) - tgui_widget_get_padding(widget, TGUI_SIDE_TOP) - tgui_widget_get_padding(widget, TGUI_SIDE_BOTTOM);
}

static inline void tgui_widget_get_inner_bounds(tgui_widget_t *widget, tgui_rect_t *rect) {
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);
	tgui_rect_init(rect, x, y, width, height);
}

void tgui_widget_set_parent(tgui_widget_t *child, tgui_widget_t *parent);
void tgui_widget_remove_parent(tgui_widget_t *child);
void tgui_container_single_calculate_sizes(tgui_widget_t *widget);
void tgui_container_single_allocate_space(tgui_widget_t *widget);

#endif
