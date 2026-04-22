#include <expander.h>
#include <label.h>

TOBJECT_DEFINE_CLASS(tgui_expander, TGUI_EXPANDER, tgui_box_get_type())

static void tgui_expander_remove_child(tgui_widget_t *parent, tgui_widget_t *child) {
	tgui_expander_t *expander = TGUI_EXPANDER_CAST(parent);
	if (expander->title == child) {
		expander->title = NULL;
	}
	if (expander->child == child) {
		expander->child = NULL;
	}
}

static void tgui_expander_click(tobject_t *tobject) {
	tgui_expander_t *expander = TGUI_EXPANDER_CAST(tobject);
	if (tgui_expander_is_expanded(expander)) {
		tgui_expander_unexpand(expander);
	} else {
		tgui_expander_expand(expander);
	}
}

static int tgui_expander_constructor(void *object) {
	tgui_expander_get_parent_class()->constructor(object);
	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_connect_signal(widget, "click", TCALLBACK_CAST(tgui_expander_click), NULL);
	return 0;
}

static void tgui_expander_class_init(tgui_expander_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->remove_child = tgui_expander_remove_child;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_expander_constructor;
}

tgui_expander_t *tgui_expander_new(void) {
	return tobject_new(tgui_expander_get_type());

}

void tgui_expander_set_title_text(tgui_expander_t *expander, const char *text) {
	if (tgui_widget_is_type(expander->title, tgui_label_get_type())) {
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
	tgui_box_prepend_widget(TGUI_BOX_CAST(expander), child);
	expander->title = child;
}

void tgui_expander_set_child(tgui_expander_t *expander, tgui_widget_t *child) {
	// destroy child if we already have one
	if (expander->child) {
		tgui_widget_destroy(expander->child);
	}
	tgui_box_append_widget(TGUI_BOX_CAST(expander), child);
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
