#include <scrolledwindow.h>

TOBJECT_DEFINE_CLASS(tgui_scrolled_window, TGUI_SCROLLED_WINDOW, tgui_widget_get_type())

static void tgui_scrolled_window_calculate_sizes(tgui_widget_t *widget) {
	tgui_scrolled_window_t *scrolled_window = TGUI_SCROLLED_WINDOW_CAST(widget);

	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(scrolled_window->hbar));
	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(scrolled_window->vbar));
	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(scrolled_window->viewport));

	widget->min_width  = TGUI_WIDGET_CAST(scrolled_window->viewport)->min_width;
	widget->min_height = TGUI_WIDGET_CAST(scrolled_window->viewport)->min_height;
	widget->pref_width  = TGUI_WIDGET_CAST(scrolled_window->viewport)->pref_width;
	widget->pref_height = TGUI_WIDGET_CAST(scrolled_window->viewport)->pref_height;

	switch (scrolled_window->vpolicy) {
	case TGUI_SCROLLED_WINDOW_POLICY_ALWAYS:
		widget->pref_width += TGUI_WIDGET_CAST(scrolled_window->vbar)->pref_width;
		// fallthrough
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		widget->min_width += TGUI_WIDGET_CAST(scrolled_window->vbar)->min_width;
		// fallthrough
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		break;
	}

	switch (scrolled_window->hpolicy) {
	case TGUI_SCROLLED_WINDOW_POLICY_ALWAYS:
		widget->pref_height += TGUI_WIDGET_CAST(scrolled_window->vbar)->pref_height;
		// fallthrough
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		widget->min_height += TGUI_WIDGET_CAST(scrolled_window->hbar)->min_height;
		// fallthrough
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		break;
	}
}

static int tgui_scrolled_window_constructor(void *object) {
	tgui_scrolled_window_get_parent_class()->constructor(object);

	tgui_scrolled_window_t *scrolled_window = TGUI_SCROLLED_WINDOW_CAST(object);
	scrolled_window->hbar = tgui_scrollbar_new(TGUI_ORIENTATION_HORIZONTAL);
	scrolled_window->vbar = tgui_scrollbar_new(TGUI_ORIENTATION_VERTICAL);
	scrolled_window->viewport = tgui_viewport_new();
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->hbar), TGUI_WIDGET_CAST(scrolled_window));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->vbar), TGUI_WIDGET_CAST(scrolled_window));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->viewport), TGUI_WIDGET_CAST(scrolled_window));

	return 0;
}

static void tgui_scrolled_window_class_init(tgui_scrolled_window_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_scrolled_window_calculate_sizes;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_scrolled_window_constructor;
}

tgui_scrolled_window_t *tgui_scrolled_window_new(void) {
	return tobject_new(tgui_scrolled_window_get_type());
}

void tgui_scrolled_window_set_child(tgui_scrolled_window_t *scrolled_window, tgui_widget_t *child) {
	tgui_viewport_set_child(scrolled_window->viewport, child);
}

void tgui_scrolled_window_set_hpolicy(tgui_scrolled_window_t *scrolled_window, int hpolicy) {
	scrolled_window->hpolicy = hpolicy;
}

void tgui_scrolled_window_set_vpolicy(tgui_scrolled_window_t *scrolled_window, int vpolicy) {
	scrolled_window->vpolicy = vpolicy;
}

tgui_widget_t *tgui_scrolled_window_get_child(tgui_scrolled_window_t *scrolled_window) {
	return tgui_viewport_get_child(scrolled_window->viewport);
}

int tgui_scrolled_window_get_hpolicy(tgui_scrolled_window_t *scrolled_window) {
	return scrolled_window->hpolicy;
}

int tgui_scrolled_window_get_vpolicy(tgui_scrolled_window_t *scrolled_window) {
	return scrolled_window->vpolicy;
}
