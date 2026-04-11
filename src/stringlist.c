#include <stringlist.h>
#include <stdlib.h>
#include <string.h>

tgui_string_item_t *tgui_string_item_new(const char *str) {
	tgui_string_item_t *item = malloc(sizeof(tgui_string_item_t));
	memset(item, 0, sizeof(tgui_string_item_t));
	item->str = strdup(str);
	return item;
}

void tgui_string_item_destroy(tgui_string_item_t *item) {
	free(item->str);
	free(item);
}

const char *tgui_string_item_get(tgui_string_item_t *item) {
	return item->str;
}
