#include <widget.h>
#include <box.h>

void tgui_box_calculate_sizes(tgui_widget_t *widget) {
	tgui_box_t *box = TGUI_BOX_CAST(widget);
	long min_width = 0;
	long min_height = 0;
	long pref_width = 0;
	long pref_height = 0;

	TGUI_LIST_FOREACH(node, &box->widget.children) {
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(node);
		if (tgui_widget_is_hidden(widget)) continue;
		tgui_widget_calculate_sizes(widget);
		if (box->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
			if (min_width < widget->min_width) min_width = widget->min_width;
			min_height += widget->min_height;
			if (pref_width < widget->pref_width) pref_width = widget->pref_width;
			pref_height += widget->pref_height;
			if (widget->node.next) {
				// not last need to add spacing
				min_height  += box->spacing;
				pref_height += box->spacing;
			}
		} else {
			min_width += widget->min_width;
			if (min_height < widget->min_height) min_height = widget->min_height;
			pref_width += widget->pref_width;
			if (pref_height < widget->pref_height) pref_height = widget->pref_height;
			if (widget->node.next) {
				// not last need to add spacing
				min_width  += box->spacing;
				pref_width += box->spacing;
			}
		}
	}
	box->widget.min_width  = min_width;
	box->widget.min_height = min_height;
	box->widget.pref_width  = pref_width;
	box->widget.pref_height = pref_height;
}

void tgui_box_allocate_space(tgui_widget_t *widget) {
	tgui_box_t *box = TGUI_BOX_CAST(widget);
	if (box->widget.children.count == 0) return;
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	// first pass, find excess and the number of child that want it
	long excess = width - (box->widget.children.count - 1) * box->spacing;
	long pref_amount = 0;
	long expand_count = 0;
	TGUI_LIST_FOREACH(node, &box->widget.children) {
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(node);
		if (tgui_widget_is_hidden(widget)) continue;
		if (box->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
			excess -= widget->min_height;
			if (widget->flags & TGUI_WIDGET_VEXPAND) expand_count++;
			pref_amount += widget->pref_height - widget->min_height;
		} else {
			excess -= widget->min_width;
			if (widget->flags & TGUI_WIDGET_HEXPAND) expand_count++;
			pref_amount += widget->pref_width - widget->min_width;
		}
	}

	// now let's see if we have enought to give the preffered size of everyone
	long allocated_pref_amount = pref_amount;
	if (allocated_pref_amount > excess) {
		allocated_pref_amount = excess;
	}
	excess -= allocated_pref_amount;

	// actually give those sizes
	TGUI_LIST_FOREACH(node, &box->widget.children) {
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(node);
		if (tgui_widget_is_hidden(widget)) continue;
		long alloc_width;
		long alloc_height;
		if (box->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
			alloc_width = width;
			long pref = widget->pref_height - widget->min_height;
			alloc_height = widget->min_height;
			// avoid divide by 0
			if (pref_amount) {
				alloc_height += pref * allocated_pref_amount / pref_amount;
			}

			// do we want a bit of excess
			if (widget->flags & TGUI_WIDGET_VEXPAND) {
				alloc_height += excess / expand_count;
				// TODO : use remainer
			}
		} else {
			long pref = widget->pref_width - widget->min_width;
			alloc_width = widget->min_width;
			// avoid divide by 0
			if (pref_amount) {
				alloc_width += pref * allocated_pref_amount / pref_amount;
			}

			// do we want a bit of excess
			if (widget->flags & TGUI_WIDGET_HEXPAND) {
				alloc_width += excess / expand_count;
				// TODO : use remainer
			}
			alloc_height = height;
		}
		tgui_widget_allocate_space(widget, x, y, alloc_width, alloc_height);
		if (box->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
			y += alloc_height + box->spacing;
		} else {
			x += alloc_width + box->spacing;
		}
	}
}

static tgui_widget_class_t box_class = {
	.name = "box",
	.size = sizeof(tgui_box_t),
	.calculate_sizes = tgui_box_calculate_sizes,
	.allocate_space  = tgui_box_allocate_space,
};

tgui_box_t *tgui_box_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&box_class);
	if (!widget) return NULL;

	return TGUI_BOX_CAST(widget);
}

void tgui_box_append_widget(tgui_box_t *box, tgui_widget_t *widget) {
	tgui_widget_set_parent(widget, TGUI_WIDGET_CAST(box));
}
