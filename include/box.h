#ifndef TGUI_BOX_H
#define TGUI_BOX_H

#include "widget.h"

typedef struct tgui_box {
	tgui_widget_t widget;
	int spacing;
} tgui_box_t;

#define TGUI_BOX_CAST(w) TGUI_CONTAINER_OF(w, tgui_box_t, widget)

tgui_box_t *tgui_box_new(void);
void tgui_box_calculate_sizes(tgui_widget_t *widget);
void tgui_box_allocate_space(tgui_widget_t *widget);

static inline void tgui_box_set_spacing(tgui_box_t *box, int spacing) {
	box->spacing = spacing;
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(box));
}

void tgui_box_append_widget(tgui_box_t *box, tgui_widget_t *widget);

#endif
