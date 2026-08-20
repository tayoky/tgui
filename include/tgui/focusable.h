#ifndef TGUI_FOCUSABLE_H
#define TGUI_FOCUSABLE_H

#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_focusable, TGUI_FOCUSABLE, tgui_widget)

/**
 * @brief represent a widget that can have focus
 */
struct tgui_focusable {
	tgui_widget_t widget;
};

#endif
