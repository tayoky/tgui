#include <stdlib.h>
#include <string.h>
#include <widget.h>
#include <render.h>

tgui_widget_t *tgui_widget_new(tgui_widget_class_t *class) {
	if (class->new) {
		return class->new();
	} else {
		tgui_widget_t *widget = malloc(class->size);
		memset(widget, 0, class->size);
		widget->class = class;
		widget->font_size = 12;
		tgui_widget_mark_dirty(widget);
		tgui_widget_mark_dirty_size(widget);
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

void tgui_widget_render(tgui_widget_t *widget) {
	if (!widget) return;
	if (!tgui_widget_is_dirty(widget)) {
		return;
	}
	widget->flags &= TGUI_WIDGET_DIRTY;
	tgui_render_widget_base(widget);
	if (widget->class->render) {
		widget->class->render(widget);
	}
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		tgui_widget_render(child);
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
