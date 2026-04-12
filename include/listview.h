#ifndef TGUI_LIST_VIEW_H
#define TGUI_LIST_VIEW_H

#include "factory.h"
#include "list.h"
#include "widget.h"

typedef struct tgui_list_view {
	tgui_widget_t widget;
	tgui_list_t *list;
	tgui_factory_t *factory;
} tgui_list_view_t;

#define TGUI_LIST_VIEW_CAST(w) TGUI_CONTAINER_OF(w, tgui_list_view_t, widget)

tgui_list_view_t *tgui_list_view_new(tgui_factory_t *factory, tgui_list_t *list);
void tgui_list_view_update(tgui_list_view_t *list_view);
void tgui_list_view_set_factory(tgui_list_view_t *list_view, tgui_factory_t *factory);
void tgui_list_view_set_list(tgui_list_view_t *list_view, tgui_list_t *list);
tgui_factory_t *tgui_list_view_get_factory(tgui_list_view_t *list_view);
tgui_list_t *tgui_list_view_get_list(tgui_list_view_t *list_view);

#endif
