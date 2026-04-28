#include <textview.h>

TOBJECT_DEFINE_CLASS(tgui_text_view, TGUI_TEXT_VIEW, tgui_widget_get_type())

static int tgui_text_view_destructor(void *object) {
	tgui_text_view_t *text_view = TGUI_TEXT_VIEW_CAST(object);
	tobject_free(TOBJECT_CAST(text_view->buffer));

	return tgui_text_view_get_parent_class()->destructor(object);
}

static void tgui_text_view_class_init(tgui_text_view_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_text_view_destructor;
}

tgui_text_view_t *tgui_text_view_new(void) {
	return tobject_new(tgui_text_view_get_type());
}

void tgui_text_view_set_buffer(tgui_text_view_t *text_view, tgui_text_buffer_t *buffer) {
	tobject_free(TOBJECT_CAST(text_view->buffer));
	text_view->buffer = tgui_text_buffer_ref(buffer);
}

tgui_text_buffer_t *tgui_text_view_get_buffer(tgui_text_view_t *text_view) {
	return text_view->buffer;
}
