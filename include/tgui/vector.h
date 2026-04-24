#ifndef TGUI_VECTOR_H
#define TGUI_VECTOR_H

#include "listmodel.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_vector, TGUI_VECTOR, tgui_list_model)

struct tgui_vector {
	tgui_list_model_t list_model;
	size_t count;
	size_t capacity;
	void **array;
};

tgui_vector_t *tgui_vector_new(void);
void tgui_vector_append_mul(tgui_vector_t *vector, void **item, size_t count);
void tgui_vector_insert_mul(tgui_vector_t *vector, size_t index, void **item, size_t count);
void tgui_vector_remove_mul(tgui_vector_t *vector, size_t index, size_t count);

static inline void tgui_vector_append(tgui_vector_t *vector, void *item) {
	tgui_vector_append_mul(vector, &item, 1);
}

static inline void tgui_vector_insert(tgui_vector_t *vector, size_t index, void *item) {
	tgui_vector_insert_mul(vector, index, &item, 1);
}

static inline void tgui_vector_remove(tgui_vector_t *vector, size_t index) {
	tgui_vector_remove_mul(vector, index, 1);
}

#endif
