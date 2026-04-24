#include <stringlist.h>
#include <stdlib.h>
#include <stdio.h>

TOBJECT_DEFINE_CLASS(tgui_string_list, TGUI_STRING_LIST, tgui_vector_get_type())

static int tgui_string_list_destructor(void *object) {
	tgui_list_model_t *list = TGUI_LIST_MODEL_CAST(object);
	size_t i=0;
	char *str;
	while ((str = tgui_list_model_get_item(list, i++))) {
		free(str);
	}

	return tgui_string_list_get_parent_class()->destructor(object);
}

static void tgui_string_list_class_init(tgui_string_list_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_string_list_destructor;
}

tgui_string_list_t *tgui_string_list_new(const char *const *strings) {
	tgui_string_list_t *list = tobject_new(tgui_string_list_get_type());
	if (!list) return NULL;

	if (strings) {
		size_t strings_count = 0;
		while (strings[strings_count]) {
			strings_count++;
		}
		tgui_string_list_append_mul(list, strings, strings_count);
	}

	return list;
}

void tgui_string_list_append_mul(tgui_string_list_t *list, const char *const *strings, size_t count) {
	char *dups[count];
	for (size_t i=0; i<count; i++) {
		dups[i] = strdup(strings[i]);
	}
	tgui_vector_append_mul(TGUI_VECTOR_CAST(list), (void**)dups, count);
}

void tgui_string_list_insert_mul(tgui_string_list_t *list, size_t index, const char *const *strings, size_t count) {
	char *dups[count];
	for (size_t i=0; i<count; i++) {
		dups[i] = strdup(strings[i]);
	}
	tgui_vector_insert_mul(TGUI_VECTOR_CAST(list), index, (void**)dups, count);
}

void tgui_string_list_remove_mul(tgui_string_list_t *list, size_t index, size_t count) {
	for (size_t i=0; i<count; i++) {
		char *str = tgui_list_model_get_item(TGUI_LIST_MODEL_CAST(list), index + i);
		free(str);
	}
	tgui_vector_remove_mul(TGUI_VECTOR_CAST(list), index, count);
}
