#ifndef TGUI_FONT_H
#define TGUI_FONT_H

#include <stddef.h>

typedef struct tgui_font {
	size_t ref_count;
	char *name;
	void *private;
} tgui_font_t;

tgui_font_t *tgui_font_load(const char *name);
tgui_font_t *tgui_font_ref(tgui_font_t *font);
void tgui_font_release(tgui_font_t *font);
void tgui_font_set_default(tgui_font_t *font);
tgui_font_t *tgui_font_get_default(void);

#endif
