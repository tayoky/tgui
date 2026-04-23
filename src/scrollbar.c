#include <scrollbar.h>

TOBJECT_DEFINE_CLASS(tgui_scrollbar, TGUI_SCROLLBAR, tgui_box_get_type())

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

static void tgui_scrollbar_top_click(tobject_t *tobject) {
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	tgui_scrollbar_add_value(scrollbar, -12);
}

static void tgui_scrollbar_bottom_click(tobject_t *tobject) {
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	tgui_scrollbar_add_value(scrollbar, 12);
}

static void tgui_scrollbar_slider_changed(tobject_t *tobject, double *value) {
	tgui_widget_t *widget = TGUI_WIDGET_CAST(tobject);
	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(widget->parent);
	
	long l_value = *value;
	tgui_widget_send_signal(TGUI_WIDGET_CAST(scrollbar), "changed", &l_value);
}

static int tgui_scrollbar_constructor(void *object) {
	tgui_scrollbar_get_parent_class()->constructor(object);

	tgui_scrollbar_t *scrollbar = TGUI_SCROLLBAR_CAST(object);
	tgui_box_t *box = TGUI_BOX_CAST(object);
	scrollbar->top    = tgui_button_new();
	scrollbar->bottom = tgui_button_new();
	scrollbar->slider = tgui_slider_new(TGUI_ORIENTATION_VERTICAL);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(scrollbar->top), "click", TCALLBACK_CAST(tgui_scrollbar_top_click), NULL);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(scrollbar->bottom), "click", TCALLBACK_CAST(tgui_scrollbar_bottom_click), NULL);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(scrollbar->slider), "changed", TCALLBACK_CAST(tgui_scrollbar_slider_changed), NULL);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrollbar->slider), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrollbar->slider), TGUI_TRUE);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(scrollbar->top));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(scrollbar->slider));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(scrollbar->bottom));
	scrollbar->view_size = 0;
	return 0;
}

static void tgui_scrollbar_class_init(tgui_scrollbar_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->set_orientation = tgui_scrollbar_set_orientation;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_scrollbar_constructor;
}

tgui_scrollbar_t *tgui_scrollbar_new(int orientation) {
	tgui_scrollbar_t *scrollbar = tobject_new(tgui_scrollbar_get_type());
	if (!scrollbar) return NULL;
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(scrollbar), orientation);
	return scrollbar;
}

static void tgui_scrollbar_update(tgui_scrollbar_t *scrollbar) {
	tgui_slider_set_range(scrollbar->slider, 0, scrollbar->total_size - scrollbar->view_size);
	tgui_slider_set_size(scrollbar->slider, (double)scrollbar->view_size / (double)scrollbar->total_size);
}

void tgui_scrollbar_set_total_size(tgui_scrollbar_t *scrollbar, long total_size) {
	scrollbar->total_size = total_size;
	tgui_scrollbar_update(scrollbar);
}

void tgui_scrollbar_set_view_size(tgui_scrollbar_t *scrollbar, long view_size) {
	if (view_size > scrollbar->total_size) {
		view_size = scrollbar->total_size;
	}
	scrollbar->view_size = view_size;
	tgui_scrollbar_update(scrollbar);
}

void tgui_scrollbar_set_value(tgui_scrollbar_t *scrollbar, long value) {
	tgui_slider_set_value(scrollbar->slider, value);
}

void tgui_scrollbar_add_value(tgui_scrollbar_t *scrollbar, long value) {
	long old_val = tgui_scrollbar_get_value(scrollbar);
	tgui_scrollbar_set_value(scrollbar, old_val + value);
}

long tgui_scrollbar_get_value(tgui_scrollbar_t *scrollbar) {
	return tgui_slider_get_value(scrollbar->slider);
}
