#include <expander.h>
#include <label.h>

static void tgui_expander_remove_child(tgui_widget_t *parent, tgui_widget_t *child) {
	tgui_expander_t *expander = TGUI_EXPANDER_CAST(parent);
	if (expander->title == child) {
		expander->title = NULL;
	}
	if (expander->child == child) {
		expander->child = NULL;
	}
}

static int tgui_expander_click(tgui_event_t *event) {
	tgui_expander_t *expander = TGUI_EXPANDER_CAST(event->widget);
	if (tgui_expander_is_expanded(expander)) {
		tgui_expander_unexpand(expander);
	} else {
		tgui_expander_expand(expander);
	}
	return TGUI_EVENT_HANDLED;
}

static tgui_widget_class_t expander_class = {
	.name = "expander",
	.size = sizeof(tgui_expander_t),
	.remove_child = tgui_expander_remove_child,

	// the expander is basicly a box
	.calculate_sizes = tgui_box_calculate_sizes,
	.allocate_space  = tgui_box_allocate_space,
};

tgui_expander_t *tgui_expander_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&expander_class);
	if (!widget) return NULL;

	tgui_expander_t *expander = TGUI_EXPANDER_CAST(widget);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_expander_click, NULL);
	return expander;
}

void tgui_expander_set_title_text(tgui_expander_t *expander, const char *text) {
	if (tgui_widget_is_class(expander->title, "label")) {
		// we already have a label
		tgui_label_set_text(TGUI_LABEL_CAST(expander->title), text);
	} else {
		tgui_label_t *label = tgui_label_new(text);
		tgui_expander_set_title_child(expander, TGUI_WIDGET_CAST(label));
	}
}

void tgui_expander_set_title_child(tgui_expander_t *expander, tgui_widget_t *child) {
	// destroy title if we already have one
	if (expander->title) {
		tgui_widget_destroy(expander->title);
	}
	tgui_box_prepend_widget(&expander->box, child);
	expander->title = child;
}

void tgui_expander_set_child(tgui_expander_t *expander, tgui_widget_t *child) {
	// destroy child if we already have one
	if (expander->child) {
		tgui_widget_destroy(expander->child);
	}
	tgui_box_append_widget(&expander->box, child);
	expander->child = child;
	if (tgui_expander_is_expanded(expander)) {
		tgui_widget_show(child);
	} else {
		tgui_widget_hide(child);
	}
}

void tgui_expander_expand(tgui_expander_t *expander) {
	expander->expanded = 1;
	tgui_widget_show(expander->child);
}

void tgui_expander_unexpand(tgui_expander_t *expander) {
	expander->expanded = 0;
	tgui_widget_hide(expander->child);
}

int tgui_expander_is_expanded(tgui_expander_t *expander) {
	return expander->expanded;
}
