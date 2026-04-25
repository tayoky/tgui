#include <listview.h>

TOBJECT_DEFINE_CLASS(tgui_list_view, TGUI_LIST_VIEW, tgui_widget_get_type())

static size_t tgui_list_view_get_children_count(tgui_list_view_t *list_view) {
	return TGUI_WIDGET_CAST(list_view)->children.count;
}

static tgui_list_item_t *tgui_list_view_setup(tgui_list_view_t *list_view) {
	if (list_view->recycle.first) {
		// recycle an old list item
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(list_view->recycle.first);
		tgui_list_remove(&list_view->recycle, &widget->node);
		return TGUI_LIST_ITEM_CAST(widget);
	}
	return tgui_factory_setup(list_view->factory);
}

static tgui_widget_t *tgui_list_view_get_widget(tgui_list_view_t *list_view, size_t index) {
	if (index < list_view->first_index) return NULL;
	index -= list_view->first_index;
	if (index >= tgui_list_view_get_children_count(list_view)) return NULL;
	TGUI_LIST_FOREACH(node, &TGUI_WIDGET_CAST(list_view)->children) {
		if (index == 0) {
			return TGUI_WIDGET_FROM_NODE(node);
		}
		index--;
	}
	return NULL;
}

static tgui_widget_t *tgui_list_view_bind(tgui_list_view_t *list_view, size_t index) {
	if (index < list_view->first_index) return NULL;
	tgui_list_item_t *list_item = tgui_list_view_setup(list_view);

	tgui_factory_bind(list_view->factory, list_item, tgui_list_model_get_item(list_view->list, index));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(list_item), TGUI_WIDGET_CAST(list_view));

	if (index != list_view->first_index + tgui_list_view_get_children_count(list_view) - 1) {
		tgui_list_remove(&TGUI_WIDGET_CAST(list_view)->children, &TGUI_WIDGET_CAST(list_item)->node);
		tgui_widget_t *after = tgui_list_view_get_widget(list_view, index);
		tgui_list_add_before(&TGUI_WIDGET_CAST(list_view)->children, &after->node, &TGUI_WIDGET_CAST(list_item)->node);
	}
	return TGUI_WIDGET_CAST(list_item);
}

static void tgui_list_view_unbind(tgui_list_view_t *list_view, tgui_widget_t *widget) {
	tgui_factory_unbind(list_view->factory, TGUI_LIST_ITEM_CAST(widget));
	tgui_widget_remove_parent(widget);

	// now we can recycle it
	tgui_list_append(&list_view->recycle, &widget->node);
}

static void tgui_list_view_generate(tgui_list_view_t *list_view, size_t count) {
	if (!list_view->list || !list_view->factory) {
		return;
	}
	size_t items_count = tgui_list_model_get_count(list_view->list);
	if (list_view->first_index >= items_count) return;
	if (list_view->first_index + count > items_count) {
		count = items_count - list_view->first_index;
	}

	// generate only after what is aready generated
	for (size_t i=tgui_list_view_get_children_count(list_view); i<count; i++) {
		tgui_list_view_bind(list_view, list_view->first_index + i);
	}
}

static void tgui_list_view_set_first_index(tgui_list_view_t *list_view, size_t index) {
	// TODO
}

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
	tgui_list_view_generate(list_view, 1);
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

	tgui_list_view_generate(list_view, 10000);

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

static int tgui_list_view_destructor(void *object) {
	tgui_list_view_t *list_view = TGUI_LIST_VIEW_CAST(object);
	tgui_list_view_set_list(list_view, NULL);

	return tgui_list_view_get_parent_class()->destructor(object);
}

static void tgui_list_view_class_init(tgui_list_view_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_list_view_calculate_sizes;
	widget_class->allocate_space = tgui_list_view_allocate_space;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_list_view_destructor;
}

static void tgui_list_view_list_changed(tobject_t *tobject, tgui_list_model_update_t *update, tgui_list_view_t *list_view) {
	(void)tobject;

	// no need to generate if it's not on s screen
	if (update->index >= list_view->first_index + tgui_list_view_get_children_count(list_view)) {
		// it's below we don't care
		return;
	}
	// TODO : update the list
	
	tgui_widget_t *widget = tgui_list_view_get_widget(list_view, update->index);
	if (!widget) {
		// if offscreen we remove the top elements
		widget = TGUI_WIDGET_FROM_NODE(TGUI_WIDGET_CAST(list_view)->children.first);
	}
	for (size_t i=0; i<update->removed&&widget; i++) {
		tgui_widget_t *next = NULL;
		if (widget->node.next) {
			next = TGUI_WIDGET_FROM_NODE(widget->node.next);
		}

		tgui_list_view_unbind(list_view, widget);

		widget = next;
	}

	for (size_t i=0; i<update->added; i++) {
		tgui_list_view_bind(list_view, update->index + i);
	}
}

static void tgui_list_view_list_destroy(tobject_t *tobject, void *event, tgui_list_view_t *list_view) {
	(void)tobject;
	(void)event;
	tgui_list_view_set_list(list_view, NULL);
}

tgui_list_view_t *tgui_list_view_new(tgui_factory_t *factory, tgui_list_model_t *list) {
	tgui_list_view_t *list_view = tobject_new(tgui_list_view_get_type());
	if (!list_view) return NULL;
	list_view->factory = factory;
	tgui_list_view_set_list(list_view, list);
	return list_view;
}

static void tgui_list_view_unbind_all(tgui_list_view_t *list_view) {
	while (TGUI_WIDGET_CAST(list_view)->children.first) {
		tgui_list_view_unbind(list_view, TGUI_WIDGET_FROM_NODE(TGUI_WIDGET_CAST(list_view)->children.first));
	}
}

static void tgui_list_view_destroy_all(tgui_list_view_t *list_view) {
	tgui_list_view_unbind_all(list_view);
	while (list_view->recycle.first) {
		tgui_widget_destroy(TGUI_WIDGET_FROM_NODE(list_view->recycle.first));
	}
}

void tgui_list_view_set_factory(tgui_list_view_t *list_view, tgui_factory_t *factory) {
	tgui_list_view_destroy_all(list_view);
	list_view->factory = factory;
}

void tgui_list_view_set_list(tgui_list_view_t *list_view, tgui_list_model_t *list) {
	if (list_view->list) {
		tgui_list_model_disconnect_signal(list_view->list, "changed", list_view->changed_callback);
		tgui_list_model_disconnect_signal(list_view->list, "destroy", list_view->destroy_callback);
	}
	tgui_list_view_unbind_all(list_view);
	list_view->list = list;
	if (list) {
		list_view->changed_callback = tgui_list_model_connect_signal(list, "changed", TCALLBACK_CAST(tgui_list_view_list_changed), list_view);
		list_view->destroy_callback = tgui_list_model_connect_signal(list, "destroy", TCALLBACK_CAST(tgui_list_view_list_destroy), list_view);
	}
}

tgui_factory_t *tgui_list_view_get_factory(tgui_list_view_t *list_view) {
	return list_view->factory;
}

tgui_list_model_t *tgui_list_view_get_list(tgui_list_view_t *list_view) {
	return list_view->list;
}
