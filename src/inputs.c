#include <inputs.h>
#include <events.h>

void tgui_input_click(tgui_surface_t *surface, int button, long x, long y) {
	if (!surface) return;
	surface->mouse_pressed = 1;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	tgui_surface_set_focus(surface, widget);
	if (!widget) widget = TGUI_WIDGET_CAST(surface);
	tgui_event_click_t event = {
		.button = button,
		.x = x,
		.y = y,
	};
	tgui_widget_send_parent_signal(widget, "click", &event);
}

void tgui_input_unclick(tgui_surface_t *surface, int button, long x, long y) {
	if (!surface) return;
	surface->mouse_pressed = 0;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	tgui_event_unclick_t event = {
		.button = button,
		.x = x,
		.y = y,
	};
	tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "unclick", &event);
}

static void update_hover(tgui_surface_t *surface, tgui_widget_t *widget) {
	tgui_widget_t *common_parent = NULL;
	tgui_widget_t *current = widget;
	while (current && !tgui_widget_get_state(current, TGUI_STATE_HOVER)) {
		tgui_widget_set_state(current, TGUI_STATE_HOVER, TGUI_TRUE);
		current = current->parent;
	}
	
	// the common parent is a parent in common between
	// the old hover widget and the new one
	if (current && tgui_widget_get_state(current, TGUI_STATE_HOVER)) {
		// we have a common parent
		common_parent = current;
	}

	current = surface->hover;
	while (current && tgui_widget_get_state(current, TGUI_STATE_HOVER) && current != common_parent) {
		tgui_widget_set_state(current, TGUI_STATE_HOVER, TGUI_FALSE);
		current = current->parent;
	}
	surface->hover = widget;
}

void tgui_input_move(tgui_surface_t *surface, long x, long y) {
	if (!surface) return;
	x /= surface->scaling;
	y /= surface->scaling;
	tgui_widget_t *widget = tgui_widget_get_at(TGUI_WIDGET_CAST(surface), x, y);
	if (widget != surface->hover) {
		update_hover(surface, widget);
	}
	tgui_event_move_t event = {
		.abs_x = x,
		.abs_y = y,
		.is_pressed = surface->mouse_pressed,
	};
	tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "move", &event);
}

void tgui_input_focus(tgui_surface_t *surface) {
	if (!surface) return;
	tgui_widget_send_signal(TGUI_WIDGET_CAST(surface), "focus", NULL);
}

void tgui_input_unfocus(tgui_surface_t *surface) {
	if (!surface) return;
	tgui_widget_send_signal(TGUI_WIDGET_CAST(surface), "unfocus", NULL);
}

void tgui_input_key_press(tgui_surface_t *surface, long scancode, long sym) {
	if (!surface) return;
	tgui_event_press_t event = {
		.scancode = scancode,
		.sym = sym,
	};
	tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "press", &event);
}

void tgui_input_key_release(tgui_surface_t *surface, long scancode, long sym) {
	if (!surface) return;
	tgui_event_release_t event = {
		.scancode = scancode,
		.sym = sym,
	};
	tgui_widget_send_parent_signal(tgui_surface_get_focus(surface), "release", &event);
}
