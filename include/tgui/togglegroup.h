#ifndef TGUI_TOGGLE_GROUP_H
#define TGUI_TOGGLE_GROUP_H

#include "togglebutton.h"
#include "list.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_toggle_group, TGUI_TOGGLE_GROUP, tobject)

typedef struct tgui_toggle_group_element {
	tgui_list_node_t node;
	tgui_toggle_group_t *group;
	tgui_toggle_button_t *button;
	size_t toggled;
	size_t destroy;
} tgui_toggle_group_element_t;

struct tgui_toggle_group {
	tobject_t tobject;
	tgui_list_t elements;
	tgui_toggle_group_element_t *current;
	int always;
};

tgui_toggle_group_t *tgui_toggle_group_new(void);
tgui_toggle_group_element_t *tgui_toggle_group_add(tgui_toggle_group_t *group, tgui_toggle_button_t *toggle_button);
void tgui_toggle_group_remove(tgui_toggle_group_t *group, tgui_toggle_group_element_t *element);

/**
 * @brief set/unset the always option of a toggle group to prevent direct toggling off of the current toggled button
 * @param group the toggle group to set the option of
 * @param always the new value of the option
 */
void tgui_toggle_group_set_always(tgui_toggle_group_t *group, int always);

/**
 * @brief toggle all buttons inside a group to inactive
 * @param group the group in which to inactivate the buttons
 */
void tgui_toggle_group_reset(tgui_toggle_group_t *group);
static inline void tgui_toggle_group_free(tgui_toggle_group_t *group) {
	tobject_free(TOBJECT_CAST(group));
}

#endif
