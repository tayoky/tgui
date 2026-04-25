#include <listview.h>

TOBJECT_DEFINE_CLASS(tgui_list_view, TGUI_LIST_VIEW, tgui_list_base_get_type())

static void tgui_list_view_calculate_sizes(tgui_widget_t *widget) {
	// the idea is pretty simple
	// take the average size of visible widgets
	// and multiply by the number of elements
	tgui_list_view_t *list_view = TGUI_LIST_VIEW_CAST(widget);

	long min_width  = 0;
	long min_height = 0;
	long pref_width  = 0;
	long pref_height = 0;

	// make sure we have at least one children
	if (TGUI_LIST_BASE_CAST(list_view)->view_count == 0) {
		tgui_list_base_set_view_count(TGUI_LIST_BASE_CAST(list_view), 1);
	}
	if (widget->children.count == 0) return;

	if (widget->orientation == TGUI_ORIENTATION_VERTICAL) {
		long min_visible  = 0;
		long pref_visible = 0;
		TGUI_LIST_FOREACH(node, &widget->children) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
			tgui_widget_calculate_sizes(child);
			min_visible += child->min_height;
			pref_visible += child->pref_height;
			if (child->min_width > min_width) {
				min_width = child->min_width;
			}
			if (child->pref_width > pref_width) {
				pref_width = child->pref_width;
			}
		}
		min_height  = min_visible * tgui_list_model_get_count(tgui_list_view_get_list(list_view)) / widget->children.count;
		pref_height = pref_visible * tgui_list_model_get_count(tgui_list_view_get_list(list_view)) / widget->children.count;
	} else {
		long min_visible  = 0;
		long pref_visible = 0;
		TGUI_LIST_FOREACH(node, &widget->children) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
			tgui_widget_calculate_sizes(child);
			min_visible += child->min_width;
			pref_visible += child->pref_width;
			if (child->min_height > min_height) {
				min_height = child->min_height;
			}
			if (child->pref_height > pref_height) {
				pref_height = child->pref_height;
			}
		}
		min_width  = min_visible * tgui_list_model_get_count(tgui_list_view_get_list(list_view)) / widget->children.count;
		pref_width = pref_visible * tgui_list_model_get_count(tgui_list_view_get_list(list_view)) / widget->children.count;
	}

	widget->min_width  = min_width;
	widget->min_height = min_height;
	widget->pref_width  = pref_width;
	widget->pref_height = pref_height;
}

static void tgui_list_view_allocate_space(tgui_widget_t *widget) {
	tgui_list_view_t *list_view = TGUI_LIST_VIEW_CAST(widget);

	tgui_list_base_set_view_count(TGUI_LIST_BASE_CAST(list_view), 10000);

	// TODO : have an offset
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	// first pass get pref amount
	long pref_amount = 0;
	long excess = widget->orientation == TGUI_ORIENTATION_VERTICAL ? height : width;
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		if (widget->orientation == TGUI_ORIENTATION_VERTICAL) {
			excess -= child->min_height;
			pref_amount += child->pref_height - child->min_height;
		} else {
			excess -= child->min_width;
			pref_amount += child->pref_width - child->min_width;
		}
	}

	// second pass actually allocate space
	TGUI_LIST_FOREACH(node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		if (widget->orientation == TGUI_ORIENTATION_VERTICAL) {
			long pref = child->pref_height - child->min_height;
			long alloc = child->min_height;
			if (pref) {
				alloc += pref * excess / pref_amount;
			}
			tgui_widget_allocate_space(child, x, y, width, alloc);
			y += alloc;
		} else {
			long pref = child->pref_width - child->min_width;
			long alloc = child->min_width;
			if (pref) {
				alloc += pref * excess / pref_amount;
			}
			tgui_widget_allocate_space(child, x, y, alloc, width);
			x += alloc;
		}
	}
}

static void tgui_list_view_class_init(tgui_list_view_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_list_view_calculate_sizes;
	widget_class->allocate_space = tgui_list_view_allocate_space;
}

tgui_list_view_t *tgui_list_view_new(tgui_factory_t *factory, tgui_list_model_t *list) {
	tgui_list_view_t *list_view = tobject_new(tgui_list_view_get_type());
	if (!list_view) return NULL;
	tgui_list_view_set_factory(list_view, factory);
	tgui_list_view_set_list(list_view, list);
	return list_view;
}

void tgui_list_view_set_factory(tgui_list_view_t *list_view, tgui_factory_t *factory) {
	tgui_list_base_set_factory(TGUI_LIST_BASE_CAST(list_view), factory);
}

void tgui_list_view_set_list(tgui_list_view_t *list_view, tgui_list_model_t *list) {
	tgui_list_base_set_list(TGUI_LIST_BASE_CAST(list_view), list);
}

tgui_factory_t *tgui_list_view_get_factory(tgui_list_view_t *list_view) {
	return tgui_list_base_get_factory(TGUI_LIST_BASE_CAST(list_view));
}

tgui_list_model_t *tgui_list_view_get_list(tgui_list_view_t *list_view) {
	return tgui_list_base_get_list(TGUI_LIST_BASE_CAST(list_view));
}
