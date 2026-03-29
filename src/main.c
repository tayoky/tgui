#include <platform.h>
#include <theme.h>
#include <tgui.h>

static int quit;

int tgui_init(void) {
	if (tgui_platform_init() < 0) {
		return -1;
	}
	tgui_font_set_default(tgui_font_load(NULL));
	tgui_theme_load();
	quit = 0;
	return 0;
}

void tgui_fini(void) {
	tgui_font_set_default(NULL);
	tgui_platform_fini();
}

void tgui_quit(void) {
	quit = 1;
}

void tgui_main(void) {
	while (!quit) {
		TGUI_LIST_FOREACH(node, tgui_get_windows()) {
			tgui_window_t *window = TGUI_CONTAINER_OF(node, tgui_window_t, node);
			tgui_window_render(window);
		}
		tgui_platform_handle_event();
	}
}
