#include <widget.h>
#include <separator.h>
#include <paned.h>
#include <box.h>

TOBJECT_DEFINE_CLASS(tgui_paned, TGUI_PANED, tgui_box_get_type())

static void tgui_paned_calculate_sizes(tgui_widget_t *widget) {
	// use the parent class's calculate size
	TGUI_WIDGET_CLASS_CAST(tgui_paned_get_parent_class())->calculate_sizes(widget);

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

static void tgui_paned_set_orientation(tgui_widget_t *widget, int orientation) {
	tgui_paned_t *paned = TGUI_PANED_CAST(widget);
	if (orientation == TGUI_ORIENTATION_VERTICAL) {
		tgui_widget_set_orientation(TGUI_WIDGET_CAST(paned->handle), TGUI_ORIENTATION_HORIZONTAL);
	} else {
		tgui_widget_set_orientation(TGUI_WIDGET_CAST(paned->handle), TGUI_ORIENTATION_VERTICAL);
	}
}

static void tgui_paned_click(tobject_t *tobject, tgui_event_click_t *event) {
	tgui_paned_t *paned = TGUI_PANED_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	if (paned->box.widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		paned->offset = paned->amount - event->y;
	} else {
		paned->offset = paned->amount - event->x;
	}
}

static void tgui_paned_move(tobject_t *tobject, tgui_event_move_t *event) {
	tgui_paned_t *paned = TGUI_PANED_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	if (paned->box.widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		paned->amount = event->abs_y + paned->offset;
	} else {
		paned->amount = event->abs_x + paned->offset;
	}
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(paned));
}

static int tgui_paned_constructor(void *object) {
	tgui_paned_get_parent_class()->constructor(object);

	tgui_paned_t *paned = TGUI_PANED_CAST(object);
	paned->handle = tgui_separator_new(TGUI_ORIENTATION_VERTICAL);
	tgui_box_append_widget(TGUI_BOX_CAST(object), TGUI_WIDGET_CAST(paned->handle));

	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_connect_signal(widget, "click", TCALLBACK_CAST(tgui_paned_click), NULL);
	tgui_widget_connect_signal(widget, "move", TCALLBACK_CAST(tgui_paned_move), NULL);
	return 0;
}

static void tgui_paned_class_init(tgui_paned_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_paned_calculate_sizes,
	widget_class->allocate_space  = tgui_paned_allocate_space;
	widget_class->set_orientation = tgui_paned_set_orientation;
	// TODO : remove child
	
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_paned_constructor;
}

tgui_paned_t *tgui_paned_new(int orientation) {
	tgui_paned_t *paned = tobject_new(tgui_paned_get_type());
	if (!paned) return NULL;
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(paned), orientation);

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
