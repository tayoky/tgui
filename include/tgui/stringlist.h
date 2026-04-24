#ifndef TGUI_STRING_LIST_H
#define TGUI_STRING_LIST_H

#include "vector.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_string_list, TGUI_STRING_LIST, tgui_vector)

struct tgui_string_list {
	tgui_vector_t vector;
};

tgui_string_list_t *tgui_string_list_new(const char *const *strings);

void tgui_string_list_append_mul(tgui_string_list_t *list, const char *const *strings, size_t count);
void tgui_string_list_insert_mul(tgui_string_list_t *list, size_t index, const char *const *strings, size_t count);
void tgui_string_list_remove_mul(tgui_string_list_t *list, size_t index, size_t count);

static inline void tgui_string_list_append(tgui_string_list_t *list, const char *string) {
	tgui_string_list_append_mul(list, &string, 1);
}

static inline void tgui_string_list_insert(tgui_string_list_t *list, size_t index, const char *string) {
	tgui_string_list_insert_mul(list, index, &string, 1);
}

static inline void tgui_string_list_remove(tgui_string_list_t *list, size_t index) {
	tgui_string_list_remove_mul(list, index, 1);
}

#endif
