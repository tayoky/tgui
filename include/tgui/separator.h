#ifndef TGUI_SEPARATOR_H
#define TGUI_SEPARATOR_H

#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_separator, TGUI_SEPARATOR, tgui_widget)

struct tgui_separator {
	tgui_widget_t widget;
};

tgui_separator_t *tgui_separator_new(int orientation);

#endif
