#ifndef TGUI_LIST_H
#define TGUI_LIST_H

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

void tgui_list_prepend(tgui_list_t *list, tgui_list_node_t *node);
void tgui_list_append(tgui_list_t *list, tgui_list_node_t *node);
void tgui_list_remove(tgui_list_t *list, tgui_list_node_t *node);

#endif
