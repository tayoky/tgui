#ifndef TGUI_TEXT_H
#define TGUI_TEXT_H

#include "widget.h"
#include "label.h"

typedef struct tgui_text {
	tgui_widget_t widget;
	tgui_label_t *label;
	char *placeholder;
	char *text;
	unsigned int cursor_x;
	unsigned int cursor_y;
} tgui_text_t;

#define TGUI_TEXT_CAST(w) TGUI_CONTAINER_OF(w, tgui_text_t, widget)

tgui_text_t *tgui_text_new(void);
void tgui_text_set_placeholder(tgui_text_t *text, const char *placeholder);
void tgui_text_set_content(tgui_text_t *text, const char *content);
const char *tgui_text_get_placeholder(tgui_text_t *text);
const char *tgui_text_get_content(tgui_text_t *text);
void tgui_text_insert(tgui_text_t *text, const char *content);
void tgui_text_delete(tgui_text_t *text, size_t len);
long tgui_text_get_cursor_x(tgui_text_t *text);
long tgui_text_get_cursor_y(tgui_text_t *text);

#endif
