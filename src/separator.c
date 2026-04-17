#include <separator.h>

TOBJECT_DEFINE_CLASS(tgui_separator, TGUI_SEPARATOR, tgui_widget_get_type())

static void tgui_separator_set_orientation(tgui_widget_t *widget, int orientation) {
	switch (orientation) {
	case TGUI_ORIENTATION_VERTICAL:
		tgui_widget_set_vexpand(widget, TGUI_TRUE);
		tgui_widget_set_hexpand(widget, TGUI_FALSE);
		break;
	case TGUI_ORIENTATION_HORIZONTAL:
		tgui_widget_set_hexpand(widget, TGUI_TRUE);
		tgui_widget_set_vexpand(widget, TGUI_FALSE);
		break;
	}
}

static void tgui_separator_class_init(tgui_separator_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->set_orientation = tgui_separator_set_orientation;
}

tgui_separator_t *tgui_separator_new(int orientation) {
	tgui_separator_t *separator = tobject_new(tgui_separator_get_type());
	if (!separator) return NULL;
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(separator), orientation);
	return separator;
}
