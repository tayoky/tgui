#include <widget.h>
#include <button.h>
#include <label.h>
#include <icon.h>

TOBJECT_DEFINE_CLASS(tgui_button, TGUI_BUTTON, tgui_widget_get_type())

static void tgui_button_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_button_t *button = TGUI_BUTTON_CAST(widget);
	if (button->child == child) {
		button->child = NULL;
	}
}

static void tgui_button_click(tobject_t *tobject) {
	tgui_widget_t *widget = TGUI_WIDGET_CAST(tobject);
	tgui_widget_set_state(widget, TGUI_STATE_PRESSED, TGUI_TRUE);
}

static void tgui_button_unclick(tobject_t *tobject) {
	tgui_widget_t *widget = TGUI_WIDGET_CAST(tobject);
	tgui_widget_set_state(widget, TGUI_STATE_PRESSED, TGUI_FALSE);
}

static int tgui_button_constructor(void *object) {
	tgui_button_get_parent_class()->constructor(object);

	tgui_widget_connect_signal(object, "click", TCALLBACK_CAST(tgui_button_click), NULL);
	tgui_widget_connect_signal(object, "unclick", TCALLBACK_CAST(tgui_button_unclick), NULL);

	// buttons have tabstop by default
	tgui_widget_set_tabstop(object, TGUI_TRUE);

	return 0;
}

static void tgui_button_class_init(tgui_button_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->remove_child   = tgui_button_remove_child;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_button_constructor;
}

tgui_button_t *tgui_button_new(void) {
	return tobject_new(tgui_button_get_type());
}

void tgui_button_set_text(tgui_button_t *button, const char *text) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_label_get_type())) {
		// we already have a label
		tgui_label_set_text(TGUI_LABEL_CAST(child), text);
	} else {
		tgui_label_t *label = tgui_label_new(text);
		tgui_button_set_child(button, TGUI_WIDGET_CAST(label));
		tgui_widget_set_hexpand(TGUI_WIDGET_CAST(label), TGUI_TRUE);
		tgui_widget_set_vexpand(TGUI_WIDGET_CAST(label), TGUI_TRUE);
		tgui_widget_set_halign(TGUI_WIDGET_CAST(label), TGUI_ALIGN_CENTER);
		tgui_widget_set_valign(TGUI_WIDGET_CAST(label), TGUI_ALIGN_CENTER);
	}
}

void tgui_button_set_icon(tgui_button_t *button, const char *icon_name) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_icon_get_type())) {
		// we already have an icon
		tgui_icon_set(TGUI_ICON_CAST(child), icon_name);
	} else {
		tgui_icon_t *icon = tgui_icon_new(icon_name);
		tgui_button_set_child(button, TGUI_WIDGET_CAST(icon));
		tgui_widget_set_hexpand(TGUI_WIDGET_CAST(icon), TGUI_TRUE);
		tgui_widget_set_vexpand(TGUI_WIDGET_CAST(icon), TGUI_TRUE);
		tgui_widget_set_halign(TGUI_WIDGET_CAST(icon), TGUI_ALIGN_CENTER);
		tgui_widget_set_valign(TGUI_WIDGET_CAST(icon), TGUI_ALIGN_CENTER);
	}
}

void tgui_button_set_child(tgui_button_t *button, tgui_widget_t *child) {
	// destroy child if we already have one
	tgui_widget_destroy(button->child);

	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(button));
	// make sure the new child is first
	// this allow for some specials buttons such as popover buttons
	// to have other "secrets" children
	tgui_list_remove(&TGUI_WIDGET_CAST(button)->children, &child->node);
	tgui_list_prepend(&TGUI_WIDGET_CAST(button)->children, &child->node);
	button->child = child;
}

const char *tgui_button_get_text(tgui_button_t *button) {
	tgui_widget_t *child = tgui_button_get_child(button);
	if (tgui_widget_is_type(child, tgui_label_get_type())) {
		return tgui_label_get_text(TGUI_LABEL_CAST(child));
	} else {
		return NULL;
	}
}

const char *tgui_button_get_icon(tgui_button_t *button) {
	(void)button;
	// TODO
	return NULL;
}

tgui_widget_t *tgui_button_get_child(tgui_button_t *button) {
	return button->child;
}
