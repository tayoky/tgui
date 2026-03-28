#include <stddef.h>
#include <list.h>

void tgui_list_prepend(tgui_list_t *list, tgui_list_node_t *node) {
	node->next = list->first;
	node->prev = NULL;
	if (list->first) {
		list->first->prev = node;
	} else {
		list->last = node;
	}
	list->first = node;
	list->count++;
}

void tgui_list_append(tgui_list_t *list, tgui_list_node_t *node) {
	node->prev = list->last;
	node->next = NULL;
	if (list->last) {
		list->last->next = node;
	} else {
		list->first = node;
	}
	list->last = node;
	list->count++;
}

void tgui_list_remove(tgui_list_t *list, tgui_list_node_t *node) {
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		list->first = node->next;
	}

	if (node->next) {
		node->next->prev = node->prev;
	} else {
		list->last = node->prev;
	}
	list->count--;
}
