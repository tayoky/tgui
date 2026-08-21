#include <popover.h>
#include <surface.h>
#include <platform.h>

TOBJECT_DEFINE_CLASS(tgui_popover, TGUI_POPOVER, tobject_get_type())

static int tgui_popover_destructor(void *object) {
	tgui_popover_t *popover = TGUI_POPOVER_CAST(object);
	tgui_widget_destroy(TGUI_WIDGET_CAST(popover->surface));

	return tgui_popover_get_parent_class()->destructor(object);
}

static void tgui_popover_click(tobject_t *tobject, tgui_event_click_t *event) {
	tgui_widget_t *surface = TGUI_WIDGET_CAST(tobject);
	tgui_popover_t *popover = TGUI_POPOVER_CAST(TGUI_WIDGET_CAST(tobject)->layout_data);
	
	long width = tgui_widget_get_outer_width(surface);
	long height = tgui_widget_get_outer_height(surface);
	long x = event->x;
	long y = event->y;


	// a click outside the popover close it
	if (x < 0 || y < 0 || x >= width || y>= height) {
		tgui_popover_popdown(popover);
	}
}

static void tgui_popover_unfocus(tobject_t *tobject) {
	tgui_popover_t *popover = TGUI_POPOVER_CAST(TGUI_WIDGET_CAST(tobject)->layout_data);
	tgui_popover_popdown(popover);
}

static void tgui_popover_class_init(tgui_popover_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_popover_destructor;
}

tgui_popover_t *tgui_popover_new(tgui_surface_t *parent) {
	tgui_popover_t *popover = tobject_new(tgui_popover_get_type());
	if (!popover) return NULL;
	popover->parent = parent;
	return popover;
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
	if (!popover) return;
	if (!popover->popped) return;
	popover->popped = 0;
	tgui_widget_hide(TGUI_WIDGET_CAST(popover->surface));
	tgui_platform_ungrab_surface(popover->surface);
}

void tgui_popover_popup(tgui_popover_t *popover) {
	if (popover->popped) return;
	popover->popped = 1;
	if (popover->surface) {
		tgui_widget_show(TGUI_WIDGET_CAST(popover->surface));
		// force a render
		// because some platforms need a render to grab input
		tgui_surface_render(popover->surface);
		tgui_platform_grab_surface(popover->surface);
		return;
	}

	popover->surface = tgui_surface_new(1, 1, popover->parent);
	TGUI_WIDGET_CAST(popover->surface)->layout_data = popover;
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(popover->surface), "click", TCALLBACK_CAST(tgui_popover_click), NULL);
	//tgui_widget_connect_signal(TGUI_WIDGET_CAST(popover->surface), "unfocus", TCALLBACK_CAST(tgui_popover_unfocus), NULL);
	tgui_surface_set_child(popover->surface, popover->child);
	tgui_surface_set_position(popover->surface, popover->x, popover->y);
	tgui_surface_render(popover->surface);
	tgui_platform_grab_surface(popover->surface);
}

long tgui_popover_get_width(tgui_popover_t *popover) {
	if (!popover->child) return 0;
	tgui_widget_calculate_sizes(popover->child);
	return popover->child->min_width;
}

long tgui_popover_get_height(tgui_popover_t *popover) {
	if (!popover->child) return 0;
	tgui_widget_calculate_sizes(popover->child);
	return popover->child->min_height;
}
