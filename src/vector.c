#include <vector.h>
#include <stdlib.h>
#include <string.h>

TOBJECT_DEFINE_CLASS(tgui_vector, TGUI_VECTOR, tgui_list_model_get_type())

static size_t tgui_vector_get_count(tgui_list_model_t *list) {
	tgui_vector_t *vector = TGUI_VECTOR_CAST(list);
	return vector->count;
}

static void *tgui_vector_get_item(tgui_list_model_t *list, size_t index) {
	tgui_vector_t *vector = TGUI_VECTOR_CAST(list);
	if (index < vector->count) {
		return vector->array[index];
	} else {
		return NULL;
	}
}

static int tgui_vector_constructor(void *object) {
	tgui_vector_get_parent_class()->constructor(object);

	tgui_vector_t *vector = TGUI_VECTOR_CAST(object);
	vector->capacity = 1;
	vector->count = 0;
	vector->array = malloc(sizeof(void*));
	return 0;
}

static int tgui_vector_destructor(void *object) {
	tgui_vector_t *vector = TGUI_VECTOR_CAST(object);
	free(vector->array);

	return tgui_vector_get_parent_class()->destructor(object);
}

static void tgui_vector_class_init(tgui_vector_class_t *class) {
	tgui_list_model_class_t *list_model_class = TGUI_LIST_MODEL_CLASS_CAST(class);
	list_model_class->get_count = tgui_vector_get_count;
	list_model_class->get_item  = tgui_vector_get_item;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_vector_constructor;
	tobject_class->destructor  = tgui_vector_destructor;
}

tgui_vector_t *tgui_vector_new(void) {
	return tobject_new(tgui_vector_get_type());
}

static void tgui_vector_grow(tgui_vector_t *vector, size_t wanted_size) {
	if (wanted_size <= vector->capacity) return;
	size_t target_size = vector->capacity;
	while (target_size < wanted_size) {
		target_size *= 2;
	}
	vector->array = realloc(vector->array, sizeof(void*) * target_size);
	vector->capacity = target_size;
}

void tgui_vector_append_mul(tgui_vector_t *vector, void **item, size_t count) {
	tgui_vector_grow(vector, vector->count + count);
	size_t index = vector->count;
	memcpy(&vector->array[index], item, count * sizeof(void*));
	vector->count += count;
	tgui_list_model_update(TGUI_LIST_MODEL_CAST(vector), index, count, 0);
}

void tgui_vector_insert_mul(tgui_vector_t *vector, size_t index, void **item, size_t count) {
	tgui_vector_grow(vector, vector->count + count);
	memmove(&vector->array[index + count], &vector->array[index], (vector->count - index) * sizeof(void*));
	memcpy(&vector->array[index], item, count * sizeof(void*));
	vector->count += count;
	tgui_list_model_update(TGUI_LIST_MODEL_CAST(vector), index, count, 0);
}

void tgui_vector_remove_mul(tgui_vector_t *vector, size_t index, size_t count) {
	memmove(&vector->array[index], &vector->array[index + count], (vector->count - index - count) * sizeof(void*));
	vector->count -= count;
	tgui_list_model_update(TGUI_LIST_MODEL_CAST(vector), index, 0, count);
}
