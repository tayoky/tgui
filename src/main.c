#include <platform.h>
#include <theme.h>
#include <log.h>
#include <tgui.h>
#include <stdlib.h>

static int quit;

int tgui_init(void) {
	if (getenv("TGUI_DEBUG")) {
		tgui_log_set_active(TGUI_TRUE);
	}
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

void tgui_render(void) {
	TGUI_LIST_FOREACH(node, tgui_get_surfaces()) {
		tgui_surface_t *surface = TGUI_CONTAINER_OF(node, tgui_surface_t, node);
		tgui_surface_render(surface);
	}
}

void tgui_poll(void) {
	tgui_platform_handle_event();
}

void tgui_main(void) {
	while (!quit) {
		tgui_timer_update();
		tgui_render();
		tgui_poll();
	}
}

int tgui_get_fd(void) {
	return tgui_platform_get_fd();
}

void tgui_register_platform_handler(int (*handler)(void *event, void *data), void *data)  {
	tgui_platform_register_handler(handler, data);
}
