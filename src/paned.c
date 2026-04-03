#include <widget.h>
#include <separator.h>
#include <paned.h>
#include <box.h>

static void tgui_paned_calculate_sizes(tgui_widget_t *widget) {
	tgui_box_calculate_sizes(widget);
	tgui_paned_t *paned = TGUI_PANED_CAST(widget);
	if (widget->orientation == TGUI_ORIENTATION_VERTICAL) {
		if (paned->amount < paned->first->min_height) {
			paned->amount = paned->first->min_height;
		}
		// TODO : also check last has enought place
	} else {
		if (paned->amount < paned->first->min_width) {
			paned->amount = paned->first->min_width;
		}
	}
}

static void tgui_paned_allocate_space(tgui_widget_t *widget) {
	tgui_paned_t *paned = TGUI_PANED_CAST(widget);

	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	if (widget->orientation == TGUI_ORIENTATION_VERTICAL) {
		long handle_height = paned->handle->widget.min_height;
		tgui_widget_allocate_space(paned->first, x, y, width, paned->amount);
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(paned->handle), x, y + paned->amount, width, handle_height);
		tgui_widget_allocate_space(paned->last, x, y + paned->amount + handle_height, width, height - paned->amount - handle_height);
	} else {
		long handle_width = paned->handle->widget.min_width;
		tgui_widget_allocate_space(paned->first, x, y, paned->amount, height);
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(paned->handle), x + paned->amount, y, handle_width, height);
		tgui_widget_allocate_space(paned->last, x + paned->amount + handle_width, y, width - paned->amount - handle_width, height);
	}
}

static int tgui_paned_click(tgui_event_t *event) {
	tgui_paned_t *paned = TGUI_PANED_CAST(event->widget->parent);
	if (paned->box.widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		paned->offset = paned->amount - event->move.abs_y;
	} else {
		paned->offset = paned->amount - event->move.abs_x;
	}
	return TGUI_EVENT_HANDLED;
}

static int tgui_paned_move(tgui_event_t *event) {
	tgui_paned_t *paned = TGUI_PANED_CAST(event->widget->parent);
	if (paned->box.widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		paned->amount = event->move.abs_y + paned->offset;
	} else {
		paned->amount = event->move.abs_x + paned->offset;
	}
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(paned));
	return TGUI_EVENT_HANDLED;
}

static tgui_widget_class_t paned_class = {
	.size = sizeof(tgui_paned_t),
	.name = "paned",
	.calculate_sizes = tgui_paned_calculate_sizes,
	.allocate_space  = tgui_paned_allocate_space,
};

tgui_paned_t *tgui_paned_new(int orientation) {
	tgui_widget_t *widget = tgui_widget_new(&paned_class);
	if (!widget) return NULL;

	tgui_paned_t *paned = TGUI_PANED_CAST(widget);
	if (orientation == TGUI_ORIENTATION_VERTICAL) {
		paned->handle = tgui_separator_new(TGUI_ORIENTATION_HORIZONTAL);
	} else {
		paned->handle = tgui_separator_new(TGUI_ORIENTATION_VERTICAL);
	}
	tgui_widget_set_callback(TGUI_WIDGET_CAST(paned->handle), TGUI_EVENT_CLICK, tgui_paned_click, NULL);
	tgui_widget_set_callback(TGUI_WIDGET_CAST(paned->handle), TGUI_EVENT_MOVE, tgui_paned_move, NULL);
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(paned), orientation);
	tgui_box_append_widget(&paned->box, TGUI_WIDGET_CAST(paned->handle));

	return paned;
}

void tgui_paned_set_first(tgui_paned_t *paned, tgui_widget_t *widget) {
	tgui_widget_destroy(paned->first);
	tgui_box_prepend_widget(&paned->box, widget);
	paned->first = widget;
}

void tgui_paned_set_last(tgui_paned_t *paned, tgui_widget_t *widget) {
	tgui_widget_destroy(paned->last);
	tgui_box_append_widget(&paned->box, widget);
	paned->last = widget;
}
