#ifndef TGUI_FACTORY_H
#define TGUI_FACTORY_H

#include "widget.h"

typedef struct tgui_list_item {
	tgui_widget_t widget;
	void *item;
} tgui_list_item_t;

#define TGUI_LIST_ITEM_CAST(w) TGUI_CONTAINER_OF(w, tgui_list_item_t, widget)

typedef struct tgui_factory tgui_factory_t;

struct tgui_factory {
	int (*setup)(tgui_factory_t *, tgui_list_item_t *);
	int (*bind)(tgui_factory_t *, tgui_list_item_t *);
	int (*unbind)(tgui_factory_t *, tgui_list_item_t *);
};

tgui_list_item_t *tgui_factory_setup(tgui_factory_t *factory);
void tgui_factory_bind(tgui_factory_t *factory, tgui_list_item_t *list_item, void *item);
void tgui_factory_unbind(tgui_factory_t *factory, tgui_list_item_t *list_item);
void tgui_list_item_set_child(tgui_list_item_t *list_item, tgui_widget_t *child);
tgui_widget_t *tgui_list_item_get_child(tgui_list_item_t *list_item);

#endif
