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

void tgui_action_register(tgui_action_t *action);
void tgui_action_unregister(tgui_action_t *action);
void tgui_action_trigger(tgui_action_t *action, tgui_surface_t *surface, tgui_widget_t *widget, void *data);
const char *tgui_action_get_name(tgui_action_t *action);
const char *tgui_action_get_shortcut(tgui_action_t *action);
tgui_action_t *tgui_action_get_from_name(const char *name);

#endif
