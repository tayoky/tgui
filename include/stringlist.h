#ifndef TGUI_STRING_LIST_H
#define TGUI_STRING_LIST_H

#include "list.h"

typedef struct tgui_string_item {
	tgui_list_node_t *node;
	char *str;
} tgui_string_item_t;

#define TGUI_STRING_ITEM_CAST(n) ((tgui_string_item_t*)n)

tgui_string_item_t *tgui_string_item_new(const char *str);
void tgui_string_item_destroy(tgui_string_item_t *item);
const char *tgui_string_item_get(tgui_string_item_t *item);

#endif
