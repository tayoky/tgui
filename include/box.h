#ifndef TGUI_BOX_H
#define TGUI_BOX_H

#include "widget.h"

typedef struct tgui_box {
	tgui_widget_t widget;
	int orientation;
	int spacing;
} tgui_box_t;

#define TGUI_BOX_CAST(w) TGUI_CONTAINER_OF(w, tgui_box_t, widget)

#define TGUI_BOX_VERTICAL   0
#define TGUI_BOX_HORIZONTAL 1

tgui_box_t *tgui_box_new(void);

static inline void tgui_box_set_orientation(tgui_box_t *box, int orientation) {
	box->orientation = orientation;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(box));
}

static inline void tgui_box_set_spacing(tgui_box_t *box, int spacing) {
	box->spacing = spacing;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(box));
}

void tgui_box_append_widget(tgui_box_t *box, tgui_widget_t *widget);

#endif
