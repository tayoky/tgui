#ifndef TGUI_LIST_MODEL_H
#define TGUI_LIST_MODEL_H

#include "tobject.h"

TOBJECT_DECLARE_CLASS(tgui_list_model, TGUI_LIST_MODEL)

struct tgui_list_model_class {
	tobject_class_t parent_class;
	void *(*get_item)(tgui_list_model_t *list, size_t index);
	size_t (*get_count)(tgui_list_model_t *list);
};

struct tgui_list_model {
	tobject_t tobject;
};

typedef struct tgui_list_model_update {
	size_t index;
	size_t added;
	size_t removed;
} tgui_list_model_update_t;

void *tgui_list_model_get_item(tgui_list_model_t *list, size_t index);
size_t tgui_list_model_get_count(tgui_list_model_t *list);
void tgui_list_model_update(tgui_list_model_t *list, size_t index, size_t added, size_t removed);

#endif
