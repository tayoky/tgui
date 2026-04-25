#ifndef TGUI_LIST_BASE_H
#define TGUI_LIST_BASE_H

#include "widget.h"
#include "listmodel.h"
#include "factory.h"
#include "list.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_list_base, TGUI_LIST_BASE, tgui_widget)

struct tgui_list_base {
	tgui_widget_t widget;
	tgui_list_t recycle;
	tgui_list_model_t *list;
	tgui_factory_t *factory;
	size_t first_index;
	size_t view_count;
	size_t changed_callback;
	size_t destroy_callback;
};

void tgui_list_base_set_first_index(tgui_list_base_t *list_base, size_t index);
void tgui_list_base_set_view_count(tgui_list_base_t *list_base, size_t view_count);
void tgui_list_base_set_factory(tgui_list_base_t *list_base, tgui_factory_t *factory);
void tgui_list_base_set_list(tgui_list_base_t *list_base, tgui_list_model_t *list);
tgui_factory_t *tgui_list_base_get_factory(tgui_list_base_t *list_base);
tgui_list_model_t *tgui_list_base_get_list(tgui_list_base_t *list_base);

#endif
