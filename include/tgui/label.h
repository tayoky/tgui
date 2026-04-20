#ifndef TGUI_LABEL_H
#define TGUI_LABEL_H

#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_label, TGUI_LABEL, tgui_widget)
TOBJECT_DECLARE_SIMPLE_CLASS(tgui_title, TGUI_TITLE, tgui_label)

struct tgui_label {
	tgui_widget_t widget;
	char *text;
};

struct tgui_title {
	tgui_label_t *label;
};

tgui_label_t *tgui_label_new(const char *text);
tgui_label_t *tgui_title_new(const char *text);
void tgui_label_set_text(tgui_label_t *label, const char *text);
const char *tgui_label_get_text(tgui_label_t *label);

#endif
