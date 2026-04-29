#ifndef TGUI_DROPDOWN_H
#define TGUI_DROPDOWN_H

#include "popoverbutton.h"
#include "listview.h"
#include "listmodel.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_dropdown, TGUI_DROPDOWN, tgui_popover_button)

struct tgui_dropdown {
	tgui_popover_button_t popover_button;
	tgui_list_view_t *list_view;
	void *selected;
	tgui_popover_t *popover;
};

tgui_dropdown_t *tgui_dropdown_new(tgui_factory_t *factory, tgui_list_model_t *list);
tgui_dropdown_t *tgui_dropdown_from_strings(const char * const *strings);
void tgui_dropdown_set_factory(tgui_dropdown_t *dropdown, tgui_factory_t *factory);
void tgui_dropdown_set_list(tgui_dropdown_t *dropdown, tgui_list_model_t *list);
void tgui_dropdown_set_selected(tgui_dropdown_t *dropdown, void *selected);
void *tgui_dropdown_get_selected(tgui_dropdown_t *dropdown);

#endif
