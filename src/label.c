#include <stdlib.h>
#include <string.h>
#include <widget.h>
#include <render.h>
#include <label.h>
#include <platform.h>

static void tgui_label_free(tgui_widget_t *widget) {
	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	free(label->text);
}

static void tgui_label_calculate_sizes(tgui_widget_t *widget) {
	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	int width = tgui_platform_text_width(widget, label->text);
	int height = tgui_platform_text_height(widget, label->text);
	label->widget.min_width = width;
	label->widget.min_height = height;
	label->widget.pref_width = width;
	label->widget.pref_height = height;
}

static void tgui_label_render(tgui_widget_t *widget) {
	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	tgui_render_text(widget, label->widget.x, label->widget.y, label->text);
}

static tgui_widget_class_t label_class = {
	.name = "label",
	.size = sizeof(tgui_label_t),
	.free = tgui_label_free,
	.calculate_sizes = tgui_label_calculate_sizes,
	.render = tgui_label_render,
};

tgui_label_t *tgui_label_new(const char *text) {
	tgui_widget_t *widget = tgui_widget_new(&label_class);
	if (!widget) return NULL;

	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	tgui_label_set_text(label, text);
	return label;
}

void tgui_label_set_text(tgui_label_t *label, const char *text) {
	if (label->text) {
		free(label->text);
	}
	if (text) {
		label->text = strdup(text);
	} else {
		label->text = NULL;
	}
	tgui_widget_mark_dirty(TGUI_WIDGET_CAST(label));
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(label));
}
