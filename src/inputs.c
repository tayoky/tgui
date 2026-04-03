#include <inputs.h>
#include <events.h>

void tgui_input_click(tgui_window_t *window, int button, long x, long y) {
	x /= window->scaling;
	y /= window->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(window), x, y);
	tgui_window_set_focus(window, widget);
	tgui_event_t event = {
		.type = TGUI_EVENT_CLICK,
		.click = {
			.button = button,
			.x = x,
			.y = y,
		},
	};
	tgui_widget_send_event(widget, &event);
}
void tgui_input_unclick(tgui_window_t *window, int button, long x, long y) {
	x /= window->scaling;
	y /= window->scaling;
	tgui_event_t event = {
		.type = TGUI_EVENT_UNCLICK,
		.unclick = {
			.button = button,
			.x = x,
			.y = y,
		},
	};
	tgui_widget_send_event(tgui_window_get_focus(window), &event);
}

void tgui_input_move(tgui_window_t *window, long x, long y) {
	x /= window->scaling;
	y /= window->scaling;
	tgui_event_t event = {
		.type = TGUI_EVENT_MOVE,
		.move = {
			.abs_x = x,
			.abs_y = y,
		},
	};
	tgui_widget_send_event(tgui_window_get_focus(window), &event);
}

void tgui_input_key_press(tgui_window_t *window, long scancode, long sym) {
	tgui_event_t event = {
		.type = TGUI_EVENT_PRESS,
		.press = {
			.scancode = scancode,
			.sym = sym,
		},
	};
	tgui_widget_send_event(tgui_window_get_focus(window), &event);
}

void tgui_input_key_release(tgui_window_t *window, long scancode, long sym) {
	tgui_event_t event = {
		.type = TGUI_EVENT_RELEASE,
		.press = {
			.scancode = scancode,
			.sym = sym,
		},
	};
	tgui_widget_send_event(tgui_window_get_focus(window), &event);
}
