#ifndef TGUI_LIST_VIEW_H
#define TGUI_LIST_VIEW_H

#include "factory.h"
#include "listmodel.h"
#include "listbase.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_list_view, TGUI_LIST_VIEW, tgui_list_base)

struct tgui_list_view {
	tgui_list_base_t list_base;
};

tgui_list_view_t *tgui_list_view_new(tgui_factory_t *factory, tgui_list_model_t *list);
void tgui_list_view_set_factory(tgui_list_view_t *list_view, tgui_factory_t *factory);
void tgui_list_view_set_list(tgui_list_view_t *list_view, tgui_list_model_t *list);
tgui_factory_t *tgui_list_view_get_factory(tgui_list_view_t *list_view);
tgui_list_model_t *tgui_list_view_get_list(tgui_list_view_t *list_view);

#endif
