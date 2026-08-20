#ifndef TGUI_TEXT_BUFFER_H
#define TGUI_TEXT_BUFFER_H

#include "tobject.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_text_buffer, TGUI_TEXT_BUFFER, tobject)

typedef struct tgui_text_buffer_line {
	char *data;
	size_t capacity;
	size_t count;
	size_t size;
} tgui_text_buffer_line_t;

struct tgui_text_buffer {
	tobject_t tobject;
	tgui_text_buffer_line_t *lines;
	size_t lines_count;
	size_t capacity;
	size_t cursor_x;
	size_t cursor_y;
};

typedef struct tgui_text_iter {
	tgui_text_buffer_line_t *line;
	size_t index;
} tgui_text_iter_t;

tgui_text_buffer_t *tgui_text_buffer_new(void);
void tgui_text_buffer_get_start_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter);
void tgui_text_buffer_get_end_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter);
void tgui_text_buffer_get_line_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line);
void tgui_text_buffer_get_line_index(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t index);
void tgui_text_buffer_get_line_offset(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t offset);
const char *tgui_text_buffer_get_line_content(tgui_text_buffer_t *text_buffer, tgui_text_iter_t *iter);
void tgui_text_buffer_delete(tgui_text_buffer_t *buffer, tgui_text_iter_t *start, tgui_text_iter_t *end);
void tgui_text_buffer_insert_buf(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *buf, size_t size);
void tgui_text_buffer_insert(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *str);
void tgui_text_buffer_clear(tgui_text_buffer_t *buffer);
void tgui_text_buffer_set_text(tgui_text_buffer_t *buffer, const char *str);
int tgui_text_iter_cmp(tgui_text_iter_t *a, tgui_text_iter_t *b);

#endif
