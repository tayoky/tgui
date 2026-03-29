#include <stdlib.h>
#include <string.h>
#include <widget.h>
#include <render.h>
#include <stdio.h>

static void tgui_widget_mark_dirty_style(tgui_widget_t *widget) {
		widget->flags |= TGUI_WIDGET_DIRTY_STYLE;
}

tgui_widget_t *tgui_widget_new(tgui_widget_class_t *class) {
	if (class->new) {
		return class->new();
	} else {
		tgui_widget_t *widget = malloc(class->size);
		memset(widget, 0, class->size);
		widget->class = class;
		tgui_widget_mark_dirty(widget);
		tgui_widget_mark_dirty_size(widget);
		tgui_widget_mark_dirty_style(widget);
		tgui_style_t *style = tgui_style_get_default(class->name);
		if (style) {
			tgui_widget_add_style(widget, style);
		}
		return widget;
	}
}

void tgui_widget_destroy(tgui_widget_t *widget) {
	if (!widget) return;
	tgui_widget_remove_parent(widget);

	// destroy children
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		tgui_widget_destroy(child);
	}

	if (widget->class->free) {
		widget->class->free(widget);
	}
	// TODO : free styles
	free(widget);
}

void tgui_widget_calculate_sizes(tgui_widget_t *widget) {
	// do not recalculate if useless
	if (!tgui_widget_is_dirty_size(widget)) {
		return;
	}
	widget->flags &= ~TGUI_WIDGET_DIRTY_SIZE;
	if (widget->class->calculate_sizes) {
		widget->class->calculate_sizes(widget);

		tgui_style_t *style = tgui_widget_get_current_style(widget);

		// add margin to sizes
		widget->min_width  += widget->left_margin + widget->right_margin;
		widget->min_height += widget->top_margin + widget->bottom_margin;
		widget->pref_width  += widget->left_margin + widget->right_margin;
		widget->pref_height += widget->top_margin + widget->bottom_margin;

		// add padding to sizes
		widget->min_width  += widget->left_padding + widget->right_padding;
		widget->min_height += widget->top_padding + widget->bottom_padding;
		widget->pref_width  += widget->left_padding + widget->right_padding;
		widget->pref_height += widget->top_padding + widget->bottom_padding;

		// add border to sizes
		for (int side=0; side<4; side++) {
			if (style->border_style[side] == TGUI_BORDER_NONE) {
				continue;
			}
			int width = style->border_width[side];
			switch (side) {
			case TGUI_SIDE_LEFT:
			case TGUI_SIDE_RIGHT:
				widget->min_width += width;
				widget->pref_width += width;
				break;
			case TGUI_SIDE_TOP:
			case TGUI_SIDE_BOTTOM:
				widget->min_height += width;
				widget->pref_height += width;
				break;
			}
		}
	}
	printf("got min size of %ldx%ld for %s\n", widget->min_width, widget->min_height, widget->class->name);
}

void tgui_widget_allocate_space(tgui_widget_t *widget, long x, long y, long width, long height) {
	printf("allocate %ldx%ld at %ld %ld for %s\n", width, height, x, y, widget->class->name);
	long new_x = x;
	long new_width = width;
	if ((widget->flags & TGUI_WIDGET_HEXPAND) && width > widget->pref_width) {
		new_width = widget->pref_width;

		// how much excess do we have ?
		long excess = width - widget->pref_width;
		// what do we do with this exxess ?
		switch (widget->halign) {
		case TGUI_ALIGN_FILL:
			new_width += excess;
			break;
		case TGUI_ALIGN_LEFT:
			break;
		case TGUI_ALIGN_CENTER:
			new_x += excess / 2;
			break;
		case TGUI_ALIGN_RIGHT:
			new_x += excess;
			break;
		}
	} else if (width > widget->pref_width) {
		new_width = widget->pref_width;
	}

	long new_y = y;
	long new_height = height;
	if ((widget->flags & TGUI_WIDGET_VEXPAND) && height > widget->pref_height) {
		new_height = widget->pref_height;

		// how much excess do we have ?
		long excess = height - widget->pref_height;
		// what do we do with this exxess ?
		switch (widget->valign) {
		case TGUI_ALIGN_FILL:
			new_height += excess;
			break;
		case TGUI_ALIGN_TOP:
			break;
		case TGUI_ALIGN_CENTER:
			new_y += excess / 2;
			break;
		case TGUI_ALIGN_BOTTOM:
			new_y += excess;
			break;
		}
	} else if (height > widget->pref_height) {
		new_height = widget->pref_height;
	}


	if (widget->x == new_x && widget->width == new_width && widget->y == new_y && widget->height == new_height) {
		// nothing changed
		return;
	}

	// we will need to redraw this
	widget->x = new_x;
	widget->width = new_width;
	widget->y = new_y;
	widget->height = new_height;
	tgui_widget_mark_dirty(widget);

	if (widget->class->allocate_space) {
		widget->class->allocate_space(widget);
	}
}

int tgui_widget_is_class(tgui_widget_t *widget, const char *class_name) {
	if (!widget) return 0;
	return !strcmp(widget->class->name, class_name);
}

void tgui_widget_set_id(tgui_widget_t *widget, const char *id) {
	if (widget->id) free(widget->id);
	if (id) {
		widget->id = strdup(id);
	} else {
		widget->id = NULL;
	}
}

static void tgui_widget_render_raw(tgui_widget_t *widget) {
	if (!widget) return;
	if (tgui_widget_is_hidden(widget)) return;
	widget->flags &= ~TGUI_WIDGET_DIRTY;
	tgui_render_widget_base(widget);
	if (widget->class->render) {
		widget->class->render(widget);
	}
	widget->flags &= ~TGUI_WIDGET_DIRTY_CHILD;
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		tgui_widget_render_raw(child);
	}
}

void tgui_widget_render(tgui_widget_t *widget) {
	if (!widget) return;
	if (tgui_widget_is_hidden(widget)) return;
	if (tgui_widget_is_dirty(widget)) {
		tgui_widget_render_raw(widget);
		return;
	}
	if (tgui_widget_has_dirty_child(widget)) {
		widget->flags &= ~TGUI_WIDGET_DIRTY_CHILD;
		TGUI_LIST_FOREACH(node, &widget->children) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
			tgui_widget_render(child);
		}
	}
}

void tgui_widget_remove_parent(tgui_widget_t *child) {
	if (!child) return;
	tgui_widget_t *parent = child->parent;
	if (!parent) return;
	if (parent->class->remove_child) {
		parent->class->remove_child(parent, child);
	}
	child->parent = NULL;
	tgui_list_remove(&parent->children, &child->node);
	tgui_widget_mark_dirty_size(parent);
	tgui_widget_mark_dirty(parent);
}

void tgui_widget_set_parent(tgui_widget_t *child, tgui_widget_t *parent) {
	if (!child) return;
	// make sure we don't already have a parent
	tgui_widget_remove_parent(child);
	child->parent = parent;
	tgui_list_append(&parent->children, &child->node);
	tgui_widget_mark_dirty_size(parent);
	tgui_widget_mark_dirty(child);
}

void tgui_widget_set_callback(tgui_widget_t *widget, int type, tgui_callback_t callback, void *data) {
	tgui_event_set_callback(widget->callbacks, type, callback, data);
}

int tgui_widget_send_event(tgui_widget_t *widget, tgui_event_t *event) {
	while (widget) {
		event->widget = widget;
		if (tgui_event_report(widget->callbacks, event)) return TGUI_EVENT_HANDLED;
		widget = widget->parent;
	}
	return TGUI_EVENT_NOT_HANDLED;
}

tgui_widget_t *tgui_widget_get_at(tgui_widget_t *parent, long x, long y) {
	TGUI_LIST_FOREACH(node, &parent->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		if (x < child->x || x >= child->x + child->width) {
			continue;
		}
		if (y < child->y || y >= child->y + child->height) {
			continue;
		}
		tgui_widget_t *sub_child = tgui_widget_get_at(child, x - child->x, y - child->y);
		if (sub_child) return sub_child;
		return child;
	}
	return NULL;
}

static void add_style(tgui_list_t *list, tgui_style_t *style) {
	tgui_style_node_t *node = malloc(sizeof(tgui_style_node_t));
	node->style = tgui_style_ref(style);
	tgui_list_prepend(list, &node->node);
}

void tgui_widget_add_state_style(tgui_widget_t *widget, char state, tgui_style_t *style) {
	if (!style) return;
	add_style(&widget->state_styles[(int)state], style);
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_style(widget);
}

void tgui_widget_add_style(tgui_widget_t *widget, tgui_style_t *style) {
	if (!style) return;
	add_style(&widget->styles, style);
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_style(widget);
}

static void remove_style(tgui_list_t *list, tgui_style_t *style) {
	TGUI_LIST_FOREACH(node, list) {
		tgui_style_node_t *style_node = TGUI_CONTAINER_OF(node, tgui_style_node_t, node);
		if (style_node->style != style) {
			continue;
		}
		tgui_list_remove(list, &style_node->node);
		tgui_style_release(style);
		free(style_node);
	}
}

void tgui_widget_remove_state_style(tgui_widget_t *widget, char state, tgui_style_t *style) {
	if (!style) return;
	remove_style(&widget->state_styles[(int)state], style);
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_style(widget);
}

void tgui_widget_remove_style(tgui_widget_t *widget, tgui_style_t *style) {
	if (!style) return;
	remove_style(&widget->styles, style);
	tgui_widget_mark_dirty(widget);
	tgui_widget_mark_dirty_style(widget);
}

tgui_list_t *tgui_widget_get_styles(tgui_widget_t *widget) {
	return &widget->styles;
}

#define CHECK_PTR(ptr) if(style->ptr) {\
	dest_style->ptr = style->ptr;\
}

static void tgui_widget_get_current_style_recur(tgui_widget_t *widget, tgui_style_t *dest_style) {
	if (widget->parent) {
		tgui_widget_get_current_style_recur(widget->parent, dest_style);
	}

	// apply all styles
	TGUI_LIST_FOREACH(node, tgui_widget_get_styles(widget)) {
		tgui_style_t *style = TGUI_STYLE_FROM_NODE(node);
		CHECK_PTR(color);
		CHECK_PTR(background_color);
		CHECK_PTR(font);
		for (int i=0; i<4; i++) {
			CHECK_PTR(border_color[i]);
			if (style->border_style[i] != TGUI_BORDER_UNDEFINED) {
				dest_style->border_style[i] = style->border_style[i];
			}
			if (style->border_width_flags & (1 << i)) {
				dest_style->border_width[i] = style->border_width[i];
			}
				
		}
	}
	// TODO : apply state flags
}

tgui_style_t *tgui_widget_get_current_style(tgui_widget_t *widget) {
	tgui_style_t *style = &widget->cache_style;
	if (!(widget->flags & TGUI_WIDGET_DIRTY_STYLE)) {
		return style;
	}
	widget->flags &= ~TGUI_WIDGET_DIRTY_STYLE;
	memset(style, 0, sizeof(tgui_style_t));
	// defaults
	style->font_size = 12;
	style->font = tgui_font_get_default();
	for (int i=0; i<4; i++) {
		style->border_style[i] = TGUI_BORDER_NONE;
	}
	static tgui_color_t *color = NULL;
	if (!color) color = tgui_color_new_rgb(0, 0, 0);
	style->color = color;
	tgui_widget_get_current_style_recur(widget, style);
	return style;
}

unsigned int tgui_widget_get_border_size(tgui_widget_t *widget, int side);


tgui_color_t *tgui_widget_get_border_color(tgui_style_t *style, int side);


char tgui_widget_get_border_widget(tgui_widget_t *widget, int side);


tgui_color_t *tgui_widget_get_color(tgui_widget_t *widget) {
	return tgui_widget_get_current_style(widget)->color;
}

tgui_color_t *tgui_widget_get_background_color(tgui_widget_t *widget) {
	return tgui_widget_get_current_style(widget)->background_color;
}

tgui_font_t *tgui_widget_get_font(tgui_widget_t *widget) {
	return tgui_widget_get_current_style(widget)->font;
}

unsigned int tgui_widget_get_font_size(tgui_widget_t *widget) {
	return tgui_widget_get_current_style(widget)->font_size;
}

void tgui_container_single_calculate_sizes(tgui_widget_t *widget) {
	if (!widget->children.first) {
empty:
		widget->min_width = 0;
		widget->min_height = 0;
		widget->pref_width = 0;
		widget->pref_height = 0;
		return;
	}
	tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(widget->children.first);
	if (tgui_widget_is_hidden(child)) goto empty;
	tgui_widget_calculate_sizes(child);
	widget->min_width  = child->min_width;
	widget->min_height = child->min_height;
	widget->pref_width  = child->min_width;
	widget->pref_height = child->pref_height;
}
