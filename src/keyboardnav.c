#include <keyboardnav.h>
#include <inputs.h>
#include <events.h>

// keyboard navigation system

// TODO : maybee set clicked
static void tgui_keyboardnav_press(tgui_widget_t *widget, tgui_event_press_t *event, tgui_keyboardnav_t *keyboardnav) {
	(void)keyboardnav;
	if (event->sym == TGUI_KEY_SPACE || event->sym == TGUI_KEY_ENTER) {
		// send a click from keyboard
		tgui_event_click_t click_event = {
			.x = 0,
			.y = 0,
			.button = TGUI_BUTTON_KEYBOARD,
		};
		tgui_surface_t *surface = tgui_widget_get_surface(widget); 
		tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "click", &click_event);
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
