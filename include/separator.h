#ifndef TGUI_SEPARATOR_H
#define TGUI_SEPARATOR_H

#include "widget.h"

typedef struct tgui_separator {
	tgui_widget_t widget;
} tgui_separator_t;

#define TGUI_SEPARATOR_CAST(w) TGUI_CONTAINER_OF(w, tgui_separator_t, widget)

tgui_separator_t *tgui_separator_new(int orientation);


#endif
