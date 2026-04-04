#ifndef TGUI_SCROLLBAR_H
#define TGUI_SCROLLBAR_H

#include "widget.h"
#include "button.h"

typedef struct tgui_scrollbar {
	tgui_widget_t widget;
} tgui_scrollbar_t;

#define TGUI_SCROLLBAR_CAST(w) TGUI_CONTAINER_OF(w, tgui_scrollbar_t, widget)



#endif
