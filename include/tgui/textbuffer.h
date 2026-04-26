#ifndef TGUI_TEXT_BUFFER_H
#define TGUI_TEXT_BUFFER_H

#include "tobject.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_text_buffer, TGUI_TEXT_BUFFER, tgui_tobject)

typedef struct tgui_text_buffer_node {
	struct tgui_text_buffer *parent;
	size_t size;  // byte count
	size_t count; // char count
	size_t lines_count;
	union {
		struct {
			struct tgui_text_buffer *left;
			struct tgui_text_buffer *right;
		} node;
		struct {
			char *data;
		} leaf;
	};
	char is_leaf;
} tgui_text_buffer_node_t;

struct tgui_text_buffer {
	tobject_t tobject;
	tgui_text_buffer_node_t *root;
};

typedef struct tgui_text_iter {
	tgui_text_buffer_node_t *leaf;
	size_t index;
} tgui_text_iter_t;

tgui_text_buffer_t *tgui_text_buffer_new(void);
void tgui_text_buffer_get_start_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter);
void tgui_text_buffer_get_end_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter);
void tgui_text_buffer_get_line_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line);
void tgui_text_buffer_get_line_index(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t index);
void tgui_text_buffer_get_line_offset(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t offset);
void tgui_text_buffer_get_index(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t index);
void tgui_text_buffer_get_offset(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t offset);
void tgui_text_buffer_delete(tgui_text_buffer_t *buffer, tgui_text_iter_t *start, tgui_text_iter_t *end);
void tgui_text_buffer_insert_buf(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *buf, size_t size);
void tgui_text_buffer_insert(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *str);

#endif
