#ifndef TGUI_SCROLLBAR_H
#define TGUI_SCROLLBAR_H

#include "box.h"
#include "slider.h"
#include "button.h"

/**
 * @struct tgui_scrollbar
 * A scrollbar is a special type of slider with buttons at the edges intended for scrolling or zooming.
 */
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
 * @relates tgui_scrollbar
 * @brief create a new scrollbar
 * @param orientation the orientation of the new scrollbar
 * @return a new scrollbar
 */
tgui_scrollbar_t *tgui_scrollbar_new(int orientation);

/**
 * @relates tgui_scrollbar
 * @brief set the total lenght of the content of a scrollbar
 * @param scrollbar the scrollbar to set the content lenght of
 * @param total_size the total size
 */
void tgui_scrollbar_set_total_size(tgui_scrollbar_t *scrollbar, long total_size);

/**
 * @relates tgui_scrollbar
 * @brief set the lenght of the view of a scrollbar
 * @param scrollbar the scrollbar to set the lenght of
 * @param view_size the size of the view
 */
void tgui_scrollbar_set_view_size(tgui_scrollbar_t *scrollbar, long view_size);

/**
 * @relates tgui_scrollbar
 * @brief set the value of a scrollbar
 * @param scrollbar the scrollbar to change the value of
 * @param value the new value
 */
void tgui_scrollbar_set_value(tgui_scrollbar_t *scrollbar, long value);

/**
 * @relates tgui_scrollbar
 * @brief add to the value of a scrollbar
 * @param scrollbar the scrollbar to change the value of
 * @param value how much to add to the value
 */
void tgui_scrollbar_add_value(tgui_scrollbar_t *scrollbar, long value);

/**
 * @relates tgui_scrollbar
 * @brief get the value of a scrollbar
 * @param scrollbar the scrollbar to get the value of
 * @return the value of the scrollbar
 */
long tgui_scrollbar_get_value(tgui_scrollbar_t *scrollbar);

#endif
