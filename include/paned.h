#ifndef TGUI_PANED_H
#define TGUI_PANED_H

#include "widget.h"
#include "separator.h"
#include "box.h"

typedef struct tgui_paned {
	tgui_box_t box;
	tgui_separator_t *handle;
	tgui_widget_t *first;
	tgui_widget_t *last;
	long offset;
	long amount;
} tgui_paned_t;

#define TGUI_PANED_CAST(w) TGUI_CONTAINER_OF(w, tgui_paned_t, box.widget)

tgui_paned_t *tgui_paned_new(int orientation);
void tgui_paned_set_first(tgui_paned_t *paned, tgui_widget_t *widget);
void tgui_paned_set_last(tgui_paned_t *paned, tgui_widget_t *widget);

#endif
