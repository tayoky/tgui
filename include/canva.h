#ifndef TGUI_CANVA_H
#define TGUI_CANVA_H

#include "widget.h"

/**
 * @struct tgui_canva
 * A canva is a widget on which drawing can be done by using higly platform dependant functions.
 */
typedef struct tgui_canva {
    tgui_widget_t widget;
    void *private;
} tgui_canva_t;

#define TGUI_CANVA_CAST(w) TGUI_CONTAINER_OF(w, tgui_canva_t, widget)

/**
 * @relates tgui_canva
 * @brief create a new canva
 * @return the new canva
 */
tgui_canva_t *tgui_canva_new(void);

/**
 * @relates tgui_canva
 * @brief get the platform dependant context of a canva
 * @param canva the canva to get the context of
 * @return the platform dependant canva
 */
void *tgui_canva_get_ctx(tgui_canva_t *canva);

/**
 * @relates tgui_canva
 * @brief set a region of a canva as dirty
 * @param the canva to mark as dirty
 * @param x the x coordinate of the region to mark as dirty
 * @param x the y coordinate of the region to mark as dirty
 * @param width the width of the region to mark as dirty
 * @param height the height of the region to mark as dirty
 */
void tgui_canva_set_dirty(tgui_canva_t *canva, long x, long y, long width, long height);

#endif
