#include <inputs.h>
#include <events.h>

void tgui_input_click(tgui_surface_t *surface, int button, long x, long y) {
	if (!surface) return;
	surface->mouse_pressed = 1;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	tgui_surface_set_focus(surface, widget);
	tgui_widget_set_state(widget, TGUI_STATE_PRESSED);
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
void tgui_input_unclick(tgui_surface_t *surface, int button, long x, long y) {
	if (!surface) return;
	surface->mouse_pressed = 0;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	tgui_widget_set_state(tgui_surface_get_focus(surface), TGUI_STATE_NORMAL);
	tgui_widget_set_state(widget, TGUI_STATE_HOVER);
	tgui_event_t event = {
		.type = TGUI_EVENT_UNCLICK,
		.unclick = {
			.button = button,
			.x = x,
			.y = y,
		},
	};
	tgui_widget_send_event(tgui_surface_get_focus(surface), &event);
}

void tgui_input_move(tgui_surface_t *surface, long x, long y) {
	if (!surface) return;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	tgui_widget_set_state(widget, TGUI_STATE_HOVER);
	tgui_event_t event = {
		.type = TGUI_EVENT_MOVE,
		.move = {
			.abs_x = x,
			.abs_y = y,
			.is_pressed = surface->mouse_pressed,
		},
	};
	tgui_widget_send_event(tgui_surface_get_focus(surface), &event);
}

void tgui_input_key_press(tgui_surface_t *surface, long scancode, long sym) {
	if (!surface) return;
	tgui_event_t event = {
		.type = TGUI_EVENT_PRESS,
		.press = {
			.scancode = scancode,
			.sym = sym,
		},
	};
	tgui_widget_send_event(tgui_surface_get_focus(surface), &event);
}

void tgui_input_key_release(tgui_surface_t *surface, long scancode, long sym) {
	if (!surface) return;
	tgui_event_t event = {
		.type = TGUI_EVENT_RELEASE,
		.press = {
			.scancode = scancode,
			.sym = sym,
		},
	};
	tgui_widget_send_event(tgui_surface_get_focus(surface), &event);
}
