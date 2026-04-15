#ifndef TGUI_ACTION_H
#define TGUI_ACTION_H

#include "list.h"
#include "surface.h"
#include "widget.h"

typedef struct tgui_action {
	tgui_list_node_t node;
	const char *name;
	const char *shortcut;
	int (*callback)(struct tgui_action*, tgui_surface_t *, tgui_widget_t *widget, void*);
} tgui_action_t;

#define TGUI_ACTION_CAST(n) TGUI_CONTAINER_OF(n, tgui_action_t, node)

/**
 * @relates tgui_action
 * @brief register an action
 * @param action the action to register
 */
void tgui_action_register(tgui_action_t *action);

/**
 * @relates tgui_action
 * @brief unregister an action
 * @param action the action to unregister
 */
void tgui_action_unregister(tgui_action_t *action);

/**
 * @relates tgui_action
 * @brief trigger an action
 * @param action the action to trigger
 * @param surface the surface which triggered the action
 * @param widget the widget which triggered the action or NULL
 * @param data action specific data
 */
void tgui_action_trigger(tgui_action_t *action, tgui_surface_t *surface, tgui_widget_t *widget, void *data);

/**
 * @relates tgui_action
 * @brief get the name of an action
 * @param action the action to get the name of
 * @return the name of the action
 */
const char *tgui_action_get_name(tgui_action_t *action);

/**
 * @relates tgui_action
 * @brief get the shortcut of an action
 * @param action the action to get the shortcut of
 * @return the name of the action or NULL
 */
const char *tgui_action_get_shortcut(tgui_action_t *action);

/**
 * @relates tgui_action
 * @brief get an action by its name
 * @param name the name of the action
 * @return the action whose name match or NULL
 */
tgui_action_t *tgui_action_get_from_name(const char *name);

#endif
