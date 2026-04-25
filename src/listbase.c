#include <listbase.h>

TOBJECT_DEFINE_CLASS(tgui_list_base, TGUI_LIST_BASE, tgui_widget_get_type())

static size_t tgui_list_base_get_children_count(tgui_list_base_t *list_base) {
	return TGUI_WIDGET_CAST(list_base)->children.count;
}

static tgui_list_item_t *tgui_list_base_setup(tgui_list_base_t *list_base) {
	if (list_base->recycle.first) {
		// recycle an old list item
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(list_base->recycle.first);
		tgui_list_remove(&list_base->recycle, &widget->node);
		return TGUI_LIST_ITEM_CAST(widget);
	}
	return tgui_factory_setup(list_base->factory);
}

static tgui_widget_t *tgui_list_base_get_widget(tgui_list_base_t *list_base, size_t index) {
	if (index < list_base->first_index) return NULL;
	index -= list_base->first_index;
	if (index >= tgui_list_base_get_children_count(list_base)) return NULL;
	TGUI_LIST_FOREACH(node, &TGUI_WIDGET_CAST(list_base)->children) {
		if (index == 0) {
			return TGUI_WIDGET_FROM_NODE(node);
		}
		index--;
	}
	return NULL;
}

static tgui_widget_t *tgui_list_base_bind(tgui_list_base_t *list_base, size_t index) {
	if (index < list_base->first_index) return NULL;
	tgui_list_item_t *list_item = tgui_list_base_setup(list_base);

	tgui_factory_bind(list_base->factory, list_item, tgui_list_model_get_item(list_base->list, index));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(list_item), TGUI_WIDGET_CAST(list_base));

	if (index != list_base->first_index + tgui_list_base_get_children_count(list_base) - 1) {
		tgui_list_remove(&TGUI_WIDGET_CAST(list_base)->children, &TGUI_WIDGET_CAST(list_item)->node);
		tgui_widget_t *after = tgui_list_base_get_widget(list_base, index);
		tgui_list_add_before(&TGUI_WIDGET_CAST(list_base)->children, &after->node, &TGUI_WIDGET_CAST(list_item)->node);
	}
	return TGUI_WIDGET_CAST(list_item);
}

static void tgui_list_base_unbind(tgui_list_base_t *list_base, tgui_widget_t *widget) {
	tgui_factory_unbind(list_base->factory, TGUI_LIST_ITEM_CAST(widget));
	tgui_widget_remove_parent(widget);

	// now we can recycle it
	tgui_list_append(&list_base->recycle, &widget->node);
}


static void tgui_list_base_unbind_all(tgui_list_base_t *list_base) {
	while (TGUI_WIDGET_CAST(list_base)->children.first) {
		tgui_list_base_unbind(list_base, TGUI_WIDGET_FROM_NODE(TGUI_WIDGET_CAST(list_base)->children.first));
	}
}

static void tgui_list_base_destroy_all(tgui_list_base_t *list_base) {
	tgui_list_base_unbind_all(list_base);
	while (list_base->recycle.first) {
		tgui_widget_destroy(TGUI_WIDGET_FROM_NODE(list_base->recycle.first));
	}
}

static int tgui_list_base_destructor(void *object) {
	tgui_list_base_t *list_base = TGUI_LIST_BASE_CAST(object);
	tgui_list_base_set_list(list_base, NULL);
	tgui_list_base_destroy_all(list_base);

	return tgui_list_base_get_parent_class()->destructor(object);
}

static void tgui_list_base_class_init(tgui_list_base_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_list_base_destructor;
}

static void tgui_list_base_generate(tgui_list_base_t *list_base) {
	if (!list_base->list || !list_base->factory) {
		return;
	}
	size_t items_count = tgui_list_model_get_count(list_base->list);
	if (list_base->first_index >= items_count) return;

	size_t count = list_base->view_count;
	if (list_base->first_index + count > items_count) {
		count = items_count - list_base->first_index;
	}

	// generate only after what is aready generated
	for (size_t i=tgui_list_base_get_children_count(list_base); i<count; i++) {
		tgui_list_base_bind(list_base, list_base->first_index + i);
	}
}

void tgui_list_base_set_first_index(tgui_list_base_t *list_base, size_t index) {
	// TODO : fix this
	list_base->first_index = index;
	tgui_list_base_generate(list_base);
}

void tgui_list_base_set_view_count(tgui_list_base_t *list_base, size_t view_count) {
	list_base->view_count = view_count;
	tgui_list_base_generate(list_base);
}

void tgui_list_base_set_factory(tgui_list_base_t *list_base, tgui_factory_t *factory) {
	tgui_list_base_destroy_all(list_base);
	tgui_list_base_set_view_count(list_base, 0);
	list_base->factory = factory;
}

static void tgui_list_base_list_changed(tobject_t *tobject, tgui_list_model_update_t *update, tgui_list_base_t *list_base) {
	(void)tobject;

	// no need to generate if it's not on s screen
	if (update->index >= list_base->first_index + list_base->view_count) {
		// it's below we don't care
		return;
	}
	
	tgui_widget_t *widget = tgui_list_base_get_widget(list_base, update->index);
	if (!widget) {
		// if offscreen we remove the top elements
		widget = TGUI_WIDGET_FROM_NODE(TGUI_WIDGET_CAST(list_base)->children.first);
	}

	// TODO : handle the case were the index is higher than the screen
	for (size_t i=0; i<update->removed&&widget; i++) {
		tgui_widget_t *next = NULL;
		if (widget->node.next) {
			next = TGUI_WIDGET_FROM_NODE(widget->node.next);
		}

		tgui_list_base_unbind(list_base, widget);

		widget = next;
	}

	for (size_t i=0; i<update->added; i++) {
		tgui_list_base_bind(list_base, update->index + i);
	}
}

static void tgui_list_base_list_destroy(tobject_t *tobject, void *event, tgui_list_base_t *list_base) {
	(void)tobject;
	(void)event;
	tgui_list_base_set_list(list_base, NULL);
}

void tgui_list_base_set_list(tgui_list_base_t *list_base, tgui_list_model_t *list) {
	if (list_base->list) {
		tgui_list_model_disconnect_signal(list_base->list, "changed", list_base->changed_callback);
		tgui_list_model_disconnect_signal(list_base->list, "destroy", list_base->destroy_callback);
	}
	tgui_list_base_set_view_count(list_base, 0);
	tgui_list_base_unbind_all(list_base);
	list_base->list = list;
	if (list) {
		list_base->changed_callback = tgui_list_model_connect_signal(list, "changed", TCALLBACK_CAST(tgui_list_base_list_changed), list_base);
		list_base->destroy_callback = tgui_list_model_connect_signal(list, "destroy", TCALLBACK_CAST(tgui_list_base_list_destroy), list_base);
	}
}

tgui_factory_t *tgui_list_base_get_factory(tgui_list_base_t *list_base) {
	return list_base->factory;
}

tgui_list_model_t *tgui_list_base_get_list(tgui_list_base_t *list_base) {
	return list_base->list;
}
