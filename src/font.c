#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <font.h>
#include <platform.h>

static tgui_font_t *default_font;

tgui_font_t *tgui_font_load(const char *name) {
	tgui_font_t *font = malloc(sizeof(tgui_font_t));
	memset(font, 0, sizeof(tgui_font_t));
	font->name = name ? strdup(name) : NULL;
	font->ref_count = 0;
	if (tgui_platform_load_font(font, NULL) < 0) {
		free(font->name);
		free(font);
		return NULL;
	}
	return font;
}

tgui_font_t *tgui_font_ref(tgui_font_t *font) {
	if (font) font->ref_count++;
	return font;
}

void tgui_font_release(tgui_font_t *font) {
	if (!font) return;
	if (font->ref_count > 1) {
		font->ref_count--;
		return;
	}
	// TODO : free cache
	tgui_platform_free_font(font, NULL);
	free(font->name);
	free(font);
}

void tgui_font_set_default(tgui_font_t *font) {
	if (font == default_font) return;
	tgui_font_release(default_font);
	default_font = tgui_font_ref(font);
}

tgui_sized_font_t *tgui_font_get_sized(tgui_font_t *font, unsigned int size) {
	TGUI_LIST_FOREACH(node, &font->sizes) {
		tgui_sized_font_t *sized_font = TGUI_SIZED_FONT_CAST(node);
		if (sized_font->size == size) {
			return sized_font;
		}
	}

	tgui_sized_font_t *sized_font = malloc(sizeof(tgui_sized_font_t));
	sized_font->size = size;

	// need to load
	if (tgui_platform_load_font(font, sized_font) < 0) {
		free(sized_font);
		return NULL;
	}
	tgui_list_append(&font->sizes, &sized_font->node);
	return sized_font;
}

tgui_font_t *tgui_font_get_default(void) {
	return default_font;
}
