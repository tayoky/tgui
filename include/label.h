#ifndef TGUI_LABEL_H
#define TGUI_LABEL_H

#include "widget.h"

typedef struct tgui_label {
	tgui_widget_t widget;
	char *text;
} tgui_label_t;

#define TGUI_LABEL_CAST(w) TGUI_CONTAINER_OF(w, tgui_label_t, widget)

tgui_label_t *tgui_label_new(const char *text);
void tgui_label_set_text(tgui_label_t *label, const char *text);
const char *tgui_label_get_text(tgui_label_t *label);

#endif
