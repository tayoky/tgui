#include <log.h>
#include <stdio.h>

static int active_log = 0;

void tgui_vlog(const char *fmt, va_list args) {
	if (active_log) {
		fprintf(stderr, "libtgui : ");
		vfprintf(stderr, fmt, args);
	}
}

void tgui_log_set_active(int active) {
	active_log = active;
}
