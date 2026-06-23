#ifndef TGUI_TOGGLE_GROUP_H
#define TGUI_TOGGLE_GROUP_H

#include "togglebutton.h"
#include "list.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_toggle_group, TGUI_TOGGLE_GROUP, tobject)

struct tgui_toggle_group {
	tobject_t tobject;
	tgui_list_t elements;
};

typedef struct tgui_toggle_group_element {
	tgui_list_node_t node;
	tgui_toggle_group_t *group;
	tgui_toggle_button_t *button;
	size_t toggled;
	size_t destroy;
} tgui_toggle_group_element_t;

tgui_toggle_group_t *tgui_toggle_group_new(void);
tgui_toggle_group_element_t *tgui_toggle_group_add(tgui_toggle_group_t *group, tgui_toggle_button_t *toggle_button);
void tgui_toggle_group_remove(tgui_toggle_group_t *group, tgui_toggle_group_element_t *element);

/**
 * @brief toggle all buttons inside a group to inactive
 * @param group the group in which to inactivate the buttons
 */
void tgui_toggle_group_reset(tgui_toggle_group_t *group);
static inline void tgui_toggle_group_free(tgui_toggle_group_t *group) {
	tobject_free(TOBJECT_CAST(group));
}

#endif
