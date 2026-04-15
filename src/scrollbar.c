#include <scrollbar.h>

static void tgui_scrollbar_update_expand(tgui_scrollbar_t *scrollbar, int orientation) {
	int vexpand = (orientation == TGUI_ORIENTATION_HORIZONTAL);
	int hexpand = (orientation == TGUI_ORIENTATION_VERTICAL);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrollbar->top), hexpand);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrollbar->top), vexpand);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrollbar->bottom), hexpand);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrollbar->bottom), vexpand);
}

static void tgui_scrollbar_set_orientation(tgui_widget_t *widget, int orientation) {
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(widget);
	if (!scrollbar->slider) return;
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(scrollbar->slider), orientation);
	tgui_scrollbar_update_expand(scrollbar, orientation);
}

static int tgui_scrollbar_top_click(tgui_event_t *event) {
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(event->widget->parent);
	tgui_scrollbar_add_value(scrollbar, -12);
	return TGUI_EVENT_HANDLED;
}

static int tgui_scrollbar_bottom_click(tgui_event_t *event) {
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(event->widget->parent);
	tgui_scrollbar_add_value(scrollbar, 12);
	return TGUI_EVENT_HANDLED;
}

static tgui_widget_class_t scrollbar_class = {
	.size = sizeof(tgui_scrollbar_t),
	.name = "scrollbar",
	.calculate_sizes = tgui_box_calculate_sizes,
	.allocate_space  = tgui_box_allocate_space,
	.set_orientation = tgui_scrollbar_set_orientation,
};


tgui_scrollbar_t *tgui_scrollbar_new(int orientation) {
	tgui_widget_t *widget = tgui_widget_new(&scrollbar_class);
	if (!widget) return NULL;

	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(widget);
	tgui_widget_set_orientation(widget, orientation);
	scrollbar->top    = tgui_button_new();
	scrollbar->bottom = tgui_button_new();
	scrollbar->slider = tgui_slider_new(orientation);
	tgui_widget_set_callback(TGUI_WIDGET_CAST(scrollbar->top), TGUI_EVENT_CLICK, tgui_scrollbar_top_click, NULL);
	tgui_widget_set_callback(TGUI_WIDGET_CAST(scrollbar->bottom), TGUI_EVENT_CLICK, tgui_scrollbar_bottom_click, NULL);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrollbar->slider), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrollbar->slider), TGUI_TRUE);
	tgui_scrollbar_update_expand(scrollbar, orientation);
	tgui_box_append_widget(&scrollbar->box, TGUI_WIDGET_CAST(scrollbar->top));
	tgui_box_append_widget(&scrollbar->box, TGUI_WIDGET_CAST(scrollbar->slider));
	tgui_box_append_widget(&scrollbar->box, TGUI_WIDGET_CAST(scrollbar->bottom));
	scrollbar->view_size = 0;

	return scrollbar;
}

void tgui_scrollbar_set_total_size(tgui_scrollbar_t *scrollbar, long total_size) {
	tgui_slider_set_range(scrollbar->slider, 0, total_size);
	tgui_slider_set_size(scrollbar->slider, (double)scrollbar->view_size / (double)total_size);
	scrollbar->total_size = total_size;
}

void tgui_scrollbar_set_view_size(tgui_scrollbar_t *scrollbar, long view_size) {
	tgui_slider_set_size(scrollbar->slider, (double)view_size / (double)scrollbar->total_size);
	scrollbar->view_size = view_size;
}

void tgui_scrollbar_set_value(tgui_scrollbar_t *scrollbar, long value) {
	tgui_slider_set_value(scrollbar->slider, (double)value/(double)scrollbar->total_size);
}

void tgui_scrollbar_add_value(tgui_scrollbar_t *scrollbar, long value) {
	long old_val = tgui_scrollbar_get_value(scrollbar);
	tgui_scrollbar_set_value(scrollbar, old_val + value);
}

long tgui_scrollbar_get_value(tgui_scrollbar_t *scrollbar) {
	return tgui_slider_get_value(scrollbar->slider) * (double)scrollbar->total_size;
}
