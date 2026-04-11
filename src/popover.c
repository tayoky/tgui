#include <popover.h>
#include <surface.h>

static void tgui_popover_allocate_space(tgui_widget_t *widget) {


}

static tgui_widget_class_t popover_class = {
	.size = sizeof(tgui_popover_t),
	.name = "popover",
};

tgui_popover_t *tgui_popover_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&popover_class);
	if (!widget) return NULL;

	tgui_popover_t *popover = TGUI_POPOVER_CAST(widget);
	return popover;
}

void tgui_popover_set_child(tgui_popover_t *popover, tgui_widget_t *child) {
	if (popover->child) {
		tgui_widget_destroy(popover->child);
	}
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(popover));
	tgui_widget_set_visible(child, popover->popped);
}

void tgui_popover_set_position(tgui_popover_t *popover, long x, long y) {
	popover->x = x;
	popover->y = y;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(popover));
}

tgui_widget_t *tgui_popover_get_child(tgui_popover_t *popover) {
	return popover->child;
}

void tgui_popover_popdown(tgui_popover_t *popover) {
	popover->popped = 1;
}

void tgui_popover_popup(tgui_popover_t *popover) {
	popover->popped = 1;
}
