#include <textview.h>
#include <platform.h>
#include <render.h>

TOBJECT_DEFINE_CLASS(tgui_text_view, TGUI_TEXT_VIEW, tgui_widget_get_type())

static void tgui_text_view_calculate_sizes(tgui_widget_t *widget) {
	tgui_text_view_t *text_view = TGUI_TEXT_VIEW_CAST(widget);
	if (!text_view->buffer) return;
	long min_width = 0;
	long min_height = 0;
	size_t line = 0;
	tgui_text_iter_t end;
	tgui_text_buffer_get_end_iter(text_view->buffer, &end);
	for (;;) {
		tgui_text_iter_t iter;
		tgui_text_buffer_get_line_iter(text_view->buffer, &iter, line);
		line++;
		if (tgui_text_iter_cmp(&iter, &end) >= 00) {
			// finished
			break;
		}
		const char *content = tgui_text_buffer_get_line_content(text_view->buffer, &iter);
		int width = tgui_platform_text_width(widget, content);
		int height = tgui_platform_text_height(widget, content);
		if (width > min_width) min_width = width;
		min_height += height;
	}
	widget->min_width  = min_width;
	widget->min_height = min_height;
	widget->pref_width  = min_width;
	widget->pref_height = min_height;
}

static void tgui_text_view_render(tgui_widget_t *widget) {
	tgui_text_view_t *text_view = TGUI_TEXT_VIEW_CAST(widget);
	if (!text_view->buffer) return;
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	size_t line = 0;
	tgui_text_iter_t end;
	tgui_text_buffer_get_end_iter(text_view->buffer, &end);
	for (;;) {
		tgui_text_iter_t iter;
		tgui_text_buffer_get_line_iter(text_view->buffer, &iter, line);
		line++;
		if (tgui_text_iter_cmp(&iter, &end) >= 00) {
			// finished
			break;
		}
		const char *content = tgui_text_buffer_get_line_content(text_view->buffer, &iter);
		int height = tgui_platform_text_height(widget, content);
		tgui_render_text(widget, x, y, content);
		y += height;
	}
}

static int tgui_text_view_destructor(void *object) {
	tgui_text_view_t *text_view = TGUI_TEXT_VIEW_CAST(object);
	tobject_free(TOBJECT_CAST(text_view->buffer));

	return tgui_text_view_get_parent_class()->destructor(object);
}

static void tgui_text_view_class_init(tgui_text_view_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_text_view_calculate_sizes;
	widget_class->render          = tgui_text_view_render;
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
