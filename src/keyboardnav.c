#include <keyboardnav.h>
#include <inputs.h>
#include <events.h>

// keyboard navigation system

static int tgui_keyboardnav_focus_next_recur(tgui_surface_t *surface, tgui_widget_t *widget, int *found) {
	if (widget == tgui_surface_get_focus(surface)) {
		*found = 1;
	} else if (tgui_widget_is_hidden(widget) || tgui_widget_get_state(widget, TGUI_STATE_DISABLED)) {
		return 0;
	} else if (tgui_widget_is_focusable(widget) && *found) {
		tgui_surface_set_focus(surface, widget);
		return 1;
	}

	TGUI_LIST_FOREACH (node, &widget->children) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(node);
		if (tgui_keyboardnav_focus_next_recur(surface, child, found)) return 1;
	}
	return 0;
}

static void tgui_keyboardnav_focus_next(tgui_keyboardnav_t *keyboardnav) {
	tgui_surface_t *surface = tgui_widget_get_surface(keyboardnav->widget);
	int found = tgui_surface_get_focus(surface) == NULL;
	if (!tgui_keyboardnav_focus_next_recur(surface, TGUI_WIDGET_CAST(surface), &found)) {
		found = 1;
		tgui_keyboardnav_focus_next_recur(surface, TGUI_WIDGET_CAST(surface), &found);
	}
}

// TODO : maybee set clicked
static void tgui_keyboardnav_press(tgui_widget_t *widget, tgui_event_press_t *event, tgui_keyboardnav_t *keyboardnav) {
	if (event->sym == TGUI_KEY_SPACE || event->sym == TGUI_KEY_ENTER) {
		// send a click from keyboard
		tgui_event_click_t click_event = {
			.x = 0,
			.y = 0,
			.button = TGUI_BUTTON_KEYBOARD,
		};
		tgui_surface_t *surface = tgui_widget_get_surface(widget); 
		tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "click", &click_event);
	} else if (event->sym == TGUI_KEY_TAB) {
		tgui_keyboardnav_focus_next(keyboardnav);
	}
}

static void tgui_keyboardnav_release(tgui_widget_t *widget, tgui_event_release_t *event, tgui_keyboardnav_t *keyboardnav) {
	(void)keyboardnav;
	if (event->sym == TGUI_KEY_SPACE || event->sym == TGUI_KEY_ENTER) {
		// send a unclick from keyboard
		tgui_event_unclick_t unclick_event = {
			.x = 0,
			.y = 0,
			.button = TGUI_BUTTON_KEYBOARD,
		};
		tgui_surface_t *surface = tgui_widget_get_surface(widget); 
		tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "unclick", &unclick_event);
	}
}

static void tgui_keyboardnav_destroy(tgui_widget_t *widget, void *unused, tgui_keyboardnav_t *keyboardnav) {
	(void)unused;
	if (keyboardnav->widget == widget) {
		tgui_keyboardnav_set_widget(keyboardnav, NULL);
	}
}

void tgui_keyboardnav_set_widget(tgui_keyboardnav_t *keyboardnav, tgui_widget_t *widget) {
	if (keyboardnav->widget) {
		tgui_widget_disconnect_signal(keyboardnav->widget, "press", keyboardnav->press_callback);
		tgui_widget_disconnect_signal(keyboardnav->widget, "release", keyboardnav->release_callback);
		tgui_widget_disconnect_signal(keyboardnav->widget, "destroy", keyboardnav->destroy_callback);
	}
	keyboardnav->widget = widget;
	if (widget) {
		keyboardnav->press_callback   = tgui_widget_connect_signal(keyboardnav->widget, "press", TCALLBACK_CAST(tgui_keyboardnav_press), keyboardnav);
		keyboardnav->release_callback = tgui_widget_connect_signal(keyboardnav->widget, "release", TCALLBACK_CAST(tgui_keyboardnav_release), keyboardnav);
		keyboardnav->destroy_callback = tgui_widget_connect_signal(keyboardnav->widget, "destroy", TCALLBACK_CAST(tgui_keyboardnav_destroy), keyboardnav);
	}
}
