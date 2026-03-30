#include <text.h>
#include <label.h>
#include <string.h>
#include <stdlib.h>

// handle text input

static void tgui_text_free(tgui_widget_t *widget) {
	tgui_text_t *text = TGUI_TEXT_CAST(widget);
	free(text->placeholder);
	free(text->text);
}

static tgui_widget_class_t text_class = {
	.size = sizeof(tgui_text_t),
	.name = "text",
	.free = tgui_text_free,
	.calculate_sizes = tgui_container_single_calculate_sizes,
	.allocate_space = tgui_container_single_allocate_space,
};

tgui_text_t *tgui_text_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&text_class);
	if (!widget) return NULL;

	tgui_text_t *text = TGUI_TEXT_CAST(widget);
	text->label = tgui_label_new("");
	tgui_widget_set_parent(TGUI_WIDGET_CAST(text->label), TGUI_WIDGET_CAST(text));
	return text;
}

static void tgui_text_update_label(tgui_text_t *text) {
	if (text->text && text->text[0]) {
		tgui_label_set_text(text->label, text->text);
	} else {
		tgui_label_set_text(text->label, text->placeholder);
	}
}

void tgui_text_set_placeholder(tgui_text_t *text, const char *placeholder) {
	free(text->placeholder);
	if (placeholder) {
		text->placeholder = strdup(placeholder);
	} else {
		text->placeholder = NULL;
	}
	tgui_text_update_label(text);
}

void tgui_text_set_content(tgui_text_t *text, const char *content) {
	free(text->text);
	if (content) {
		text->text = strdup(content);
	} else {
		text->text = NULL;
	}
	text->cursor_x = 0;
	tgui_text_update_label(text);
}

const char *tgui_text_get_placeholder(tgui_text_t *text) {
	return text->placeholder;
}

const char *tgui_text_get_content(tgui_text_t *text) {
	return text->text;
}

void tgui_text_insert(tgui_text_t *text, const char *content) {
	if (text->text) {
		text->text = realloc(text->text, strlen(text->text) + strlen(content) + 1);
	} else {
		text->text = malloc(strlen(content) + 1);
		text->text[0] = '\0';
	}
	memmove(text->text + text->cursor_x + strlen(content), text->text + text->cursor_x, strlen(text->text) - text->cursor_x + 1);
	memcpy(text->text + text->cursor_x, content, strlen(content));
	text->cursor_x += strlen(content);
	tgui_text_update_label(text);
}

long tgui_text_get_cursor_x(tgui_text_t *text) {
	return text->cursor_x;
}

long tgui_text_get_cursor_y(tgui_text_t *text) {
	return text->cursor_y;
}
