#include <separator.h>

static tgui_widget_class_t separator_class = {
	.size = sizeof(tgui_separator_t),
	.name = "separator",
};

tgui_separator_t *tgui_separator_new(int orientation) {
	tgui_widget_t *widget = tgui_widget_new(&separator_class);
	if (!widget) return NULL;

	switch (orientation) {
	case TGUI_ORIENTATION_VERTICAL:
		tgui_widget_set_vexpand(widget, TGUI_TRUE);
		break;
	case TGUI_ORIENTATION_HORIZONTAL:
		tgui_widget_set_hexpand(widget, TGUI_TRUE);
		break;
	}

	tgui_separator_t *separator = TGUI_SEPARATOR_CAST(widget);
	return separator;
}
