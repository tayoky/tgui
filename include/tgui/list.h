#ifndef TGUI_LIST_H
#define TGUI_LIST_H

#include <stddef.h>

typedef struct tgui_list_node {
	struct tgui_list_node *prev;
	struct tgui_list_node *next;
} tgui_list_node_t;

typedef struct tgui_list {
	tgui_list_node_t *first;
	tgui_list_node_t *last;
	size_t count;
} tgui_list_t;

#define TGUI_LIST_FOREACH(node, list) for (tgui_list_node_t *node=(list)->first; node; node=node->next)
#define TGUI_CONTAINER_OF(ptr, type, member) ((type *)((char*)(ptr) - offsetof(type, member)))

void tgui_list_prepend(tgui_list_t *list, tgui_list_node_t *node);
void tgui_list_append(tgui_list_t *list, tgui_list_node_t *node);
void tgui_list_add_before(tgui_list_t *list, tgui_list_node_t *ref, tgui_list_node_t *node);
void tgui_list_add_after(tgui_list_t *list, tgui_list_node_t *ref, tgui_list_node_t *node);
void tgui_list_remove(tgui_list_t *list, tgui_list_node_t *node);
static inline int tgui_list_is_empty(tgui_list_t *list) {
	return list->first == NULL;
}

#endif
