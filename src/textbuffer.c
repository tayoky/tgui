#include <textbuffer.h>
#include <string.h>
#include <stdlib.h>

TOBJECT_DEFINE_CLASS(tgui_text_buffer, TGUI_TEXT_BUFFER, tobject_get_type())

static int tgui_text_buffer_constructor(void *object) {
	tgui_text_buffer_get_parent_class()->constructor(object);

	tgui_text_buffer_t *buffer = TGUI_TEXT_BUFFER_CAST(object);
	buffer->capacity = 1;
	buffer->lines_count = 0;
	buffer->lines = malloc(sizeof(tgui_text_buffer_line_t));
	tgui_text_buffer_clear(buffer);

	return 0;
}

static int tgui_text_buffer_destructor(void *object) {
	tgui_text_buffer_t *buffer = TGUI_TEXT_BUFFER_CAST(object);
	for (size_t i=0; i<buffer->lines_count; i++) {
		free(buffer->lines[i].data);
	}
	free(buffer->lines);
	return tgui_text_buffer_get_parent_class()->destructor(object);
}

static void tgui_text_buffer_class_init(tgui_text_buffer_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_text_buffer_constructor;
	tobject_class->destructor  = tgui_text_buffer_destructor;
}

static void append_buf(tgui_text_iter_t *iter, const char *str, size_t size) {
	size_t new_size = iter->line->size + size;
	if (iter->line->capacity < new_size) {
		size_t capacity = iter->line->capacity;
		while (capacity < new_size) {
			capacity *= 2;
		}
		iter->line->data = realloc(iter->line->data, capacity);
		iter->line->capacity = capacity;
	}
	memmove(iter->line->data + iter->index + size, iter->line->data + iter->index, iter->line->size - iter->index);
	memcpy(iter->line->data + iter->index, str, size);
	iter->line->size += size;
}

static tgui_text_buffer_line_t *append_line(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter) {
	size_t i = iter->line - buffer->lines + 1;
	if (buffer->lines_count + 1 > buffer->capacity) {
		buffer->capacity *= 2;
		buffer->lines = realloc(buffer->lines, buffer->capacity * sizeof(tgui_text_buffer_line_t));
	}
	memmove(buffer->lines + i + 1, buffer->lines + i, (buffer->lines_count - i) * sizeof(tgui_text_buffer_line_t));
	buffer->lines[i].data = strdup("");
	buffer->lines[i].size = 1;
	buffer->lines[i].capacity = 1;
	buffer->lines_count++;
	iter->line = &buffer->lines[i];
	iter->index = 0;

	return &buffer->lines[i];
}

static void delete(tgui_text_iter_t *iter, size_t count) {
	memmove(iter->line->data + iter->index, iter->line->data + iter->index + count, iter->line->size - iter->index - count);
	iter->line->size -= count;
}

static void delete_lines(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t count) {
	// TODO
}

int tgui_text_iter_cmp(tgui_text_iter_t *a, tgui_text_iter_t *b) {
	if (a->line != b->line) {
		return a->line > b->line ? 1 : -1;
	}
	if (a->index != b->index) {
		return a->index > b->index ? 1 : -1;
	}
	return 1;
}

tgui_text_buffer_t *tgui_text_buffer_new(void) {
	return tobject_new(tgui_text_buffer_get_type());
}

void tgui_text_buffer_get_start_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter) {
	iter->line = &buffer->lines[0];
	iter->index = 0;
}

void tgui_text_buffer_get_end_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter) {
	iter->line = &buffer->lines[buffer->lines_count-1];
	iter->index = iter->line->size - 1;
}

void tgui_text_buffer_get_line_iter(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line) {
	if (line >= buffer->lines_count) {
		tgui_text_buffer_get_end_iter(buffer, iter);
	}
	iter->index = 0;
	iter->line = &buffer->lines[line];
}

void tgui_text_buffer_get_line_index(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t index) {
	tgui_text_buffer_get_line_iter(buffer, iter, line);
	if (iter->index > 0) {
		// we hit end
		return;
	}
	if (index >= iter->line->size) {
		index = iter->line->size - 1;
	}
	iter->index = index;
}

void tgui_text_buffer_get_line_offset(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, size_t line, size_t offset) {
	tgui_text_buffer_get_line_iter(buffer, iter, line);
	if (iter->index > 0) {
		// we hit end
		return;
	}
	// TODO : move the iter to the correct offset
}

const char *tgui_text_buffer_get_line_content(tgui_text_buffer_t *text_buffer, tgui_text_iter_t *iter) {
	(void)text_buffer;
	return iter->line->data;
}

void tgui_text_buffer_delete(tgui_text_buffer_t *buffer, tgui_text_iter_t *start, tgui_text_iter_t *end) {
	// swap if end is before start
	if (tgui_text_iter_cmp(start, end) < 0) {
		tgui_text_iter_t *tmp = end;
		end = start;
		start = tmp;
	}

	while (tgui_text_iter_cmp(start, end) > 0) {
		if (start->line == end->line) {
			// we just have to delete stuff on current line
			delete(start, end->index - start->index);
			end->index = start->index;
		} else if (start->index == 0) {
			// we can delete multiple lines
			delete_lines(buffer, start, end->line - start->line);
			start->line = end->line;
		} else {
			// TODO : delete on current line
		}
	}
}

void tgui_text_buffer_insert_buf(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *buf, size_t size) {
	while (size > 0) {
		char *newline = memchr(buf, '\n', size);
		size_t count;
		if (newline) {
			count = newline - buf;
		} else {
			count = size;
		}
		append_buf(iter, buf, count);
		size -= count;
		buf += count;
		if (newline) {
			append_line(buffer, iter);
			size--;
			buf++;
		}
	}
	
}

void tgui_text_buffer_insert(tgui_text_buffer_t *buffer, tgui_text_iter_t *iter, const char *str) {
	tgui_text_buffer_insert_buf(buffer, iter, str, strlen(str));
}

void tgui_text_buffer_clear(tgui_text_buffer_t *buffer) {
	for (size_t i=0; i<buffer->lines_count; i++) {
		free(buffer->lines[i].data);
	}
	buffer->lines[0].data = strdup("");
	buffer->lines[0].size = 1;
	buffer->lines[0].capacity = 1;
	buffer->lines_count = 1;
}

void tgui_text_buffer_set_text(tgui_text_buffer_t *buffer, const char *str) {
	tgui_text_buffer_clear(buffer);
	tgui_text_iter_t start;
	tgui_text_buffer_get_start_iter(buffer, &start);
	tgui_text_buffer_insert(buffer, &start, str);
}
