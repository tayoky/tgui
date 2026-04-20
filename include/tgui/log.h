#ifndef TGUI_LOG_H
#define TGUI_LOG_H

#include <stdarg.h>

void tgui_log_set_active(int active);
void tgui_vlog(const char *fmt, va_list args);

static inline void tgui_log(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	tgui_vlog(fmt, args);
	va_end(args);
}

#endif
