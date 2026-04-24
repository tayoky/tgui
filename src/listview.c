#include <listview.h>

TOBJECT_DEFINE_CLASS(tgui_list_view, TGUI_LIST_VIEW, tgui_widget_get_type())

static void tgui_list_view_calculate_sizes(tgui_widget_t *widget) {
	// the idea is pretty simple
	// take the average size of visible widgets
	// and multiply by the number of elements
	tgui_list_view_t *list_view = TGUI_LIST_VIEW_CAST(widget);
	tgui_list_view_update(list_view);

	long min_width  = 0;
	long min_height = 0;
	long pref_width  = 0;
	long pref_height = 0;

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
		min_height  = min_visible * tgui_list_model_get_count(list_view->list) / widget->children.count;
		pref_height = pref_visible * tgui_list_model_get_count(list_view->list) / widget->children.count;
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
		min_width  = min_visible * tgui_list_model_get_count(list_view->list) / widget->children.count;
		pref_width = pref_visible * tgui_list_model_get_count(list_view->list) / widget->children.count;
	}

	widget->min_width  = min_width;
	widget->min_height = min_height;
	widget->pref_width  = pref_width;
	widget->pref_height = pref_height;
}

static void tgui_list_view_allocate_space(tgui_widget_t *widget) {
	tgui_list_view_t *list_view = TGUI_LIST_VIEW_CAST(widget);

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
	TGUI_LIST_FOREACH(node, &list_view->widget.children) {
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
	list_view->factory = factory;
	list_view->list    = list;
	return list_view;
}

void tgui_list_view_update(tgui_list_view_t *list_view) {
	if (!list_view->list) {
		return;
	}
	// TODO : actual logic
	if (list_view->widget.children.first) {
		return;
	}
	size_t i=0;
	void *item;
	while ((item = tgui_list_model_get_item(list_view->list, i++))) {
		tgui_list_item_t *list_item = tgui_factory_setup(list_view->factory);
		tgui_factory_bind(list_view->factory, list_item, item);
		tgui_widget_set_parent(TGUI_WIDGET_CAST(list_item), TGUI_WIDGET_CAST(list_view));
	}
}

void tgui_list_view_set_factory(tgui_list_view_t *list_view, tgui_factory_t *factory) {
	// TODO : destroy all widgets
	list_view->factory = factory;
}

void tgui_list_view_set_list(tgui_list_view_t *list_view, tgui_list_model_t *list) {
	// TODO : unbind all
	list_view->list = list;
}

tgui_factory_t *tgui_list_view_get_factory(tgui_list_view_t *list_view) {
	return list_view->factory;
}

tgui_list_model_t *tgui_list_view_get_list(tgui_list_view_t *list_view) {
	return list_view->list;
}
