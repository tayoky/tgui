#include <popover.h>
#include <surface.h>
#include <platform.h>

TOBJECT_DEFINE_CLASS(tgui_popover, TGUI_POPOVER, tgui_widget_get_type())

// TODO : destructor

static void tgui_popover_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_popover_t *popover = TGUI_POPOVER_CAST(widget);
	if (popover->child != child) return;
	// TODO : find a way
}

static int tgui_popover_click(tgui_event_t *event) {
	tgui_widget_t *surface = event->widget;
	tgui_popover_t *popover = TGUI_POPOVER_CAST(event->widget->layout_data);
	
	long width = tgui_widget_get_outer_width(surface);
	long height = tgui_widget_get_outer_height(surface);
	long x = event->click.x;
	long y = event->click.y;

	// a click outside the popover close it
	if (x < 0 || y < 0 || x >= width || y>= height) {
		tgui_popover_popdown(popover);
	}
	return TGUI_EVENT_HANDLED;
}

static int tgui_popover_unfocus(tgui_event_t *event) {
	tgui_popover_t *popover = TGUI_POPOVER_CAST(event->widget->layout_data);
	tgui_popover_popdown(popover);
	return TGUI_EVENT_HANDLED;
}

static void tgui_popover_class_init(tgui_popover_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->remove_child = tgui_popover_remove_child;
}

tgui_popover_t *tgui_popover_new(void) {
	return tobject_new(tgui_popover_get_type());
}

void tgui_popover_set_child(tgui_popover_t *popover, tgui_widget_t *child) {
	if (popover->child) {
		tgui_widget_destroy(popover->child);
	}
	if (popover->surface) {
		tgui_surface_set_child(popover->surface, child);
	}
	popover->child = child;
}

void tgui_popover_set_position(tgui_popover_t *popover, long x, long y) {
	popover->x = x;
	popover->y = y;
	if (popover->surface) {
		tgui_surface_set_position(popover->surface, x, y);
	}
}

tgui_widget_t *tgui_popover_get_child(tgui_popover_t *popover) {
	return popover->child;
}

void tgui_popover_popdown(tgui_popover_t *popover) {
	if (!popover->popped) return;
	popover->popped = 0;
	tgui_widget_hide(TGUI_WIDGET_CAST(popover->surface));
	tgui_platform_ungrab_surface(popover->surface);
}

void tgui_popover_popup(tgui_popover_t *popover) {
	tgui_surface_t *parent = tgui_widget_get_surface(TGUI_WIDGET_CAST(popover));
	popover->popped = 1;
	if (popover->surface) {
		tgui_platform_grab_surface(popover->surface);
		tgui_widget_show(TGUI_WIDGET_CAST(popover->surface));
		return;
	}

	tgui_widget_calculate_sizes(popover->child);
	popover->surface = tgui_surface_new(popover->child->pref_width, popover->child->pref_height, parent);
	TGUI_WIDGET_CAST(popover->surface)->layout_data = popover;
	tgui_widget_set_callback(TGUI_WIDGET_CAST(popover->surface), TGUI_EVENT_CLICK, tgui_popover_click, NULL);
	tgui_widget_set_callback(TGUI_WIDGET_CAST(popover->surface), TGUI_EVENT_UNFOCUS, tgui_popover_unfocus, NULL);
	tgui_surface_set_position(popover->surface, popover->x, popover->y);
	tgui_surface_set_child(popover->surface, popover->child);
	tgui_platform_grab_surface(popover->surface);
}
