#ifndef TGUI_SCROLLBAR_H
#define TGUI_SCROLLBAR_H

#include "box.h"
#include "slider.h"
#include "button.h"

typedef struct tgui_scrollbar {
	tgui_box_t box;
	tgui_button_t *top;
	tgui_button_t *bottom;
	tgui_slider_t *slider;
	long view_size;
	long total_size;
} tgui_scrollbar_t;

#define TGUI_SCROLLBAR_CAST(w) TGUI_CONTAINER_OF(w, tgui_scrollbar_t, box.widget)

/**
 * @brief create a new scrollbar
 * @param orientation the orientation of the new scrollbar
 * @return a new scrollbar
 */
tgui_scrollbar_t *tgui_scrollbar_new(int orientation);

/**
 * @brief set the total lenght of the content
 * @param scrollbar the scrollbar to set the content lenght of
 * @param total_size the total size
 */
void tgui_scrollbar_set_total_size(tgui_scrollbar_t *scrollbar, long total_size);

/**
 * @brief set the lenght of the view
 * @param scrollbar the scrollbar to set the lenght of
 * @param view_size the size of the view
 */
void tgui_scrollbar_set_view_size(tgui_scrollbar_t *scrollbar, long view_size);

#endif
