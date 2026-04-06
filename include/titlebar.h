#ifndef TGUI_HEADER_BAR_H
#define TGUI_HEADER_BAR_H

#include "box.h"
#include "button.h"
#include "label.h"

typedef struct tgui_title_bar {
	tgui_box_t box;
	tgui_label_t *title;
	tgui_button_t *minimize;
	tgui_button_t *maximize;
	tgui_button_t *close;
} tgui_title_bar_t;

#define TGUI_TITLE_BAR_CAST(w) TGUI_CONTAINER_OF(w, tgui_title_bar_t, box.widget)

tgui_title_bar_t *tgui_title_bar_new(void);
void tgui_title_bar_set_minimizable(tgui_title_bar_t *bar, int enabled);
void tgui_title_bar_set_maximizable(tgui_title_bar_t *bar, int enabled);
void tgui_title_bar_set_closable(tgui_title_bar_t *bar, int enabled);
void tgui_title_bar_set_title(tgui_title_bar_t *bar, const char *title);

#endif
