#ifndef TGUI_FONT_H
#define TGUI_FONT_H

#include <stddef.h>
#include "list.h"

typedef struct tgui_sized_font {
	tgui_list_node_t node;
	unsigned int size;
	void *private;
} tgui_sized_font_t;

#define TGUI_SIZED_FONT_CAST(n) ((tgui_sized_font_t*)n)

typedef struct tgui_font {
	tgui_list_t sizes;
	size_t ref_count;
	char *name;
	void *private;
} tgui_font_t;

tgui_font_t *tgui_font_load(const char *name);
tgui_font_t *tgui_font_ref(tgui_font_t *font);
void tgui_font_release(tgui_font_t *font);
void tgui_font_set_default(tgui_font_t *font);
tgui_sized_font_t *tgui_font_get_sized(tgui_font_t *font, unsigned int size);
tgui_font_t *tgui_font_get_default(void);

#endif
