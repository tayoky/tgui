#include <events.h>

void tgui_event_set_callback(tgui_callbacks_t callbacks, int type, tgui_callback_t callback, void *data) {
	callbacks[type].callback = callback;
	callbacks[type].data     = data;
}

int tgui_event_report(tgui_callbacks_t callbacks, tgui_event_t *event) {
	if (!callbacks[event->type].callback) return TGUI_EVENT_NOT_HANDLED;
	event->data = callbacks[event->type].data;
	return callbacks[event->type].callback(event);
}
