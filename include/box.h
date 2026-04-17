#ifndef TGUI_BOX_H
#define TGUI_BOX_H

#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_box, TGUI_BOX, tgui_widget)

/**
 * @struct tgui_box
 * A box is a simple container that put children top to bottom or left to right
 */
struct tgui_box {
	tgui_widget_t widget;
	int spacing;
};

/**
 * @relates tgui_box
 * @brief create a new box
 * @return the new box
 */
tgui_box_t *tgui_box_new(void);

/**
 * @relates tgui_box
 * @brief set the spacing between elements inside a box
 * @param box the box to set the spacing of
 * @param spacing the spacing between elements
 */
static inline void tgui_box_set_spacing(tgui_box_t *box, int spacing) {
	box->spacing = spacing;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(box));
}

/**
 * @relates tgui_box
 * @brief add a widget as the first child of a box
 * @param box the box to add the widget to
 * @param widget the widget to add
 */
void tgui_box_prepend_widget(tgui_box_t *box, tgui_widget_t *widget);

/**
 * @relates tgui_box
 * @brief add a widget as the last child of a box
 * @param box the box to add the widget to
 * @param widget the widget to add
 */
void tgui_box_append_widget(tgui_box_t *box, tgui_widget_t *widget);

#endif
