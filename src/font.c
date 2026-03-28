#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <font.h>
#include <platform.h>

static tgui_font_t *default_font;

tgui_font_t *tgui_font_load(const char *name) {
	tgui_font_t *font = malloc(sizeof(tgui_font_t));
	font->name = name ? strdup(name) : NULL;
	font->ref_count = 0;
	if (tgui_platform_load_font(font) < 0) {
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
	tgui_platform_free_font(font);
	free(font->name);
	free(font);
}

void tgui_font_set_default(tgui_font_t *font) {
	if (font == default_font) return;
	tgui_font_release(default_font);
	default_font = tgui_font_ref(font);
}

tgui_font_t *tgui_font_get_default(void) {
	return default_font;
}
