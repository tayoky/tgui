#include <listmodel.h>

TOBJECT_DEFINE_CLASS(tgui_list_model, TGUI_LIST_MODEL, tobject_get_type())

static int tgui_list_model_destructor(void *object) {
	tgui_list_model_t *list = TGUI_LIST_MODEL_CAST(object);
	tgui_list_model_send_signal(list, "destroy", NULL);

	return tgui_list_model_get_parent_class()->destructor(object);
}

static void tgui_list_model_class_init(tgui_list_model_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_list_model_destructor;
}

void *tgui_list_model_get_item(tgui_list_model_t *list, size_t index) {
	tgui_list_model_class_t *class = tgui_list_model_get_class(list);
	if (class->get_item) {
		return class->get_item(list, index);
	} else {
		return NULL;
	}
}
	
size_t tgui_list_model_get_count(tgui_list_model_t *list) {
	tgui_list_model_class_t *class = tgui_list_model_get_class(list);
	if (class->get_count) {
		return class->get_count(list);
	} else {
		return 0;
	}
}

void tgui_list_model_update(tgui_list_model_t *list, size_t index, size_t added, size_t removed) {
	tgui_list_model_update_t update = {
		.index = index,
		.added = added,
		.removed = removed,
	};
	tgui_list_model_send_signal(list, "changed", &update);
}
