#include <action.h>
#include <string.h>

static tgui_list_t actions;

void tgui_action_register(tgui_action_t *action) {
	tgui_list_append(&actions, &action->node);
}

void tgui_action_unregister(tgui_action_t *action) {
	tgui_list_remove(&actions, &action->node);
}

void tgui_action_trigger(tgui_action_t *action, tgui_surface_t *surface, tgui_widget_t *widget, void *data) {
	if (!action->callback) return;
	action->callback(action, surface, widget, data);
}

const char *tgui_action_get_name(tgui_action_t *action) {
	return action->name;
}

const char *tgui_action_get_shortcut(tgui_action_t *action) {
	return action->shortcut;
}

tgui_action_t *tgui_action_get_from_name(const char *name) {
	TGUI_LIST_FOREACH(node, &actions) {
		tgui_action_t *action = TGUI_ACTION_CAST(node);
		if (!strcmp(action->name, name)) {
			return action;
		}
	}
	return NULL;
}
