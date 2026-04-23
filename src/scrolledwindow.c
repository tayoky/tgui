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
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		widget->pref_width += TGUI_WIDGET_CAST(scrolled_window->vbar)->pref_width;
		widget->min_width += TGUI_WIDGET_CAST(scrolled_window->vbar)->min_width;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		break;
	}

	switch (scrolled_window->hpolicy) {
	case TGUI_SCROLLED_WINDOW_POLICY_ALWAYS:
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		widget->pref_height += TGUI_WIDGET_CAST(scrolled_window->hbar)->pref_height;
		widget->min_height += TGUI_WIDGET_CAST(scrolled_window->hbar)->min_height;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		break;
	}
}

static void tgui_scrolled_window_allocate_space(tgui_widget_t *widget) {
	tgui_scrolled_window_t *scrolled_window = TGUI_SCROLLED_WINDOW_CAST(widget);

	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	int have_hbar;
	switch (scrolled_window->hpolicy) {
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		have_hbar = 0;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_ALWAYS:
		have_hbar = 1;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		have_hbar = width < TGUI_WIDGET_CAST(scrolled_window->viewport)->pref_width;
		break;
	}

	int have_vbar;
	switch (scrolled_window->vpolicy) {
	case TGUI_SCROLLED_WINDOW_POLICY_NEVER:
		have_vbar = 0;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_ALWAYS:
		have_vbar = 1;
		break;
	case TGUI_SCROLLED_WINDOW_POLICY_AUTO:
		have_vbar = height < TGUI_WIDGET_CAST(scrolled_window->viewport)->pref_height;
		break;
	}

	tgui_widget_set_visible_no_dirty(TGUI_WIDGET_CAST(scrolled_window->hbar), have_hbar);
	tgui_widget_set_visible_no_dirty(TGUI_WIDGET_CAST(scrolled_window->vbar), have_vbar);

	if (have_hbar) {
		height -= TGUI_WIDGET_CAST(scrolled_window->hbar)->min_height;
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(scrolled_window->hbar), x, y + height, width, TGUI_WIDGET_CAST(scrolled_window->hbar)->min_height);
	}
	if (have_vbar) {
		width -= TGUI_WIDGET_CAST(scrolled_window->vbar)->min_width;
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(scrolled_window->vbar), x + width, y, TGUI_WIDGET_CAST(scrolled_window->hbar)->min_width, height);
	}

	tgui_widget_allocate_space(TGUI_WIDGET_CAST(scrolled_window->viewport), x, y, width, height);
}

static int tgui_scrolled_window_constructor(void *object) {
	tgui_scrolled_window_get_parent_class()->constructor(object);

	tgui_scrolled_window_t *scrolled_window = TGUI_SCROLLED_WINDOW_CAST(object);
	scrolled_window->hbar = tgui_scrollbar_new(TGUI_ORIENTATION_HORIZONTAL);
	scrolled_window->vbar = tgui_scrollbar_new(TGUI_ORIENTATION_VERTICAL);
	scrolled_window->viewport = tgui_viewport_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrolled_window->hbar), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrolled_window->vbar), TGUI_TRUE);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(scrolled_window->viewport), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(scrolled_window->viewport), TGUI_TRUE);
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->hbar), TGUI_WIDGET_CAST(scrolled_window));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->vbar), TGUI_WIDGET_CAST(scrolled_window));
	tgui_widget_set_parent(TGUI_WIDGET_CAST(scrolled_window->viewport), TGUI_WIDGET_CAST(scrolled_window));

	return 0;
}

static void tgui_scrolled_window_class_init(tgui_scrolled_window_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->allocate_space = tgui_scrolled_window_allocate_space;
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
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(scrolled_window));
}

void tgui_scrolled_window_set_vpolicy(tgui_scrolled_window_t *scrolled_window, int vpolicy) {
	scrolled_window->vpolicy = vpolicy;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(scrolled_window));
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
