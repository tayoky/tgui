#include <inputs.h>
#include <events.h>

void tgui_input_click(tgui_window_t *window, int button, long x, long y) {
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(window), x, y);
	tgui_event_t event = {
		.type = TGUI_EVENT_CLICK,
		.click = {
			.button = button,
			.x = x / window->scaling,
			.y = y / window->scaling,
		},
	};
	tgui_widget_send_event(widget, &event);
}
void tgui_input_unclick(tgui_window_t *window, int button, long x, long y) {
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(window), x, y);
	tgui_event_t event = {
		.type = TGUI_EVENT_UNCLICK,
		.unclick = {
			.button = button,
			.x = x / window->scaling,
			.y = y / window->scaling,
		},
	};
	tgui_widget_send_event(widget, &event);
}

void tgui_input_move(tgui_window_t *window, long x, long y) {
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(window), x, y);
}
