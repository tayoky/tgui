#include <factory.h>

static tgui_widget_class_t list_item_class = {
	.size = sizeof(tgui_list_item_t),
	.name = "list item",
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space  = tgui_container_single_allocate_space,
};

tgui_list_item_t *tgui_factory_setup(tgui_factory_t *factory) {
	tgui_widget_t *widget = tgui_widget_new(&list_item_class);
	if (!widget) return NULL;
	
	tgui_list_item_t *list_item = TGUI_LIST_ITEM_CAST(widget);
	if (factory->setup) {
		factory->setup(factory, list_item);
	}
	return list_item;
}

void tgui_factory_bind(tgui_factory_t *factory, tgui_list_item_t *list_item, void *item) {
	list_item->item = item;
	if (factory->bind) {
		factory->bind(factory, list_item);
	}
}

void tgui_factory_unbind(tgui_factory_t *factory, tgui_list_item_t *list_item) {
	if (factory->unbind) {
		factory->unbind(factory, list_item);
	}
	list_item->item = NULL;
}

void tgui_list_item_set_child(tgui_list_item_t *list_item, tgui_widget_t *child) {
	// one child at time
	tgui_widget_t *old_child = tgui_list_item_get_child(list_item);
	if (old_child) {
		tgui_widget_destroy(old_child);
	}
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(list_item));
}

tgui_widget_t *tgui_list_item_get_child(tgui_list_item_t *list_item) {
	if (list_item->widget.children.first) {
		return TGUI_WIDGET_FROM_NODE(list_item->widget.children.first);
	} else {
		return NULL;
	}
}
