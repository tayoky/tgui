#include <stdlib.h>
#include <string.h>
#include <widget.h>
#include <render.h>
#include <label.h>
#include <platform.h>

TOBJECT_DEFINE_CLASS(tgui_label, TGUI_LABEL, tgui_widget_get_type())
TOBJECT_DEFINE_CLASS(tgui_title, TGUI_TITLE, tgui_label_get_type())

static void tgui_label_calculate_sizes(tgui_widget_t *widget) {
	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	long scaling = tgui_surface_get_scaling(surface);
	int real_width = tgui_platform_text_width(widget, label->text);
	int real_height = tgui_platform_text_height(widget, label->text);
	int width  = (real_width + scaling - 1) / scaling;
	int height = (real_height + scaling - 1) / scaling;
	label->widget.min_width = width;
	label->widget.min_height = height;
	label->widget.pref_width = width;
	label->widget.pref_height = height;
}

static void tgui_label_render(tgui_widget_t *widget) {
	tgui_label_t *label = TGUI_LABEL_CAST(widget);
	tgui_render_text(widget, label->widget.x, label->widget.y, label->text);
}

static int tgui_label_destructor(void *object) {
	tgui_label_t *label = TGUI_LABEL_CAST(object);
	free(label->text);

	return tgui_label_get_parent_class()->destructor(object);
}

static void tgui_label_class_init(tgui_label_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_label_calculate_sizes,
	widget_class->render = tgui_label_render;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_label_destructor;
}

static void tgui_title_class_init(tgui_title_class_t *class) {
	(void)class;
}

tgui_label_t *tgui_label_new(const char *text) {
	tgui_label_t *label = tobject_new(tgui_label_get_type());
	if (!label) return NULL;
	tgui_label_set_text(label, text);
	return label;
}

tgui_label_t *tgui_title_new(const char *text) {

	tgui_label_t *label = tobject_new(tgui_title_get_type());
	if (!label) return NULL;
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

const char *tgui_label_get_text(tgui_label_t *label) {
	return label->text;
}
