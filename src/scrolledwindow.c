#include <scrolledwindow.h>

TOBJECT_DEFINE_CLASS(tgui_scrolled_window, TGUI_SCROLLED_WINDOW, tgui_widget_get_type())

static int tgui_scrolled_window_constructor(void *object) {
	tgui_scrolled_window_get_parent_class()->constructor(object);

	tgui_scrolled_window_t *scrolled_window = TGUI_SCROLLED_WINDOW_CAST(object);
	scrolled_window->hbar = tgui_scrollbar_new(TGUI_ORIENTATION_HORIZONTAL);
	scrolled_window->vbar = tgui_scrollbar_new(TGUI_ORIENTATION_VERTICAL);
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->hbar), TGUI_WIDGET_CAST(scrolled_window));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->vbar), TGUI_WIDGET_CAST(scrolled_window));

	return 0;
}

static void tgui_scrolled_window_class_init(tgui_scrolled_window_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_scrolled_window_constructor;
}

tgui_scrolled_window_t *tgui_scrolled_window_new(void) {
	return tobject_new(tgui_scrolled_window_get_type());
}

void tgui_scrolled_window_set_child(tgui_scrolled_window_t *scrolled_window, tgui_widget_t *child) {
	tgui_widget_destroy(scrolled_window->child);
	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(scrolled_window));
	scrolled_window->child = child;
}

void tgui_scrolled_window_set_hpolicy(tgui_scrolled_window_t *scrolled_window, int hpolicy) {
	scrolled_window->hpolicy = hpolicy;
}

void tgui_scrolled_window_set_vpolicy(tgui_scrolled_window_t *scrolled_window, int vpolicy) {
	scrolled_window->vpolicy = vpolicy;
}

tgui_widget_t *tgui_scrolled_window_get_child(tgui_scrolled_window_t *scrolled_window) {
	return scrolled_window->child;
}

int tgui_scrolled_window_get_hpolicy(tgui_scrolled_window_t *scrolled_window) {
	return scrolled_window->hpolicy;
}

int tgui_scrolled_window_get_vpolicy(tgui_scrolled_window_t *scrolled_window) {
	return scrolled_window->vpolicy;
}
