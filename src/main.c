#include <platform.h>
#include <tgui.h>

int tgui_init(void) {
	if (tgui_platform_init() < 0) {
		return -1;
	}
	tgui_font_set_default(tgui_font_load(NULL));
	return 0;
}

void tgui_fini(void) {
	tgui_font_set_default(NULL);
	tgui_platform_fini();
}

void tgui_main(void) {
	for(;;) {
		TGUI_LIST_FOREACH(node, tgui_get_windows()) {
			tgui_window_t *window = TGUI_CONTAINER_OF(node, tgui_window_t, node);
			tgui_window_render(window);
		}
		tgui_platform_handle_event();
	}
}
