#ifndef TGUI_CANVA_H
#define TGUI_CANVA_H

#include "widget.h"

typedef struct tgui_canva {
    tgui_widget_t widget;
    void *private;
} tgui_canva_t;

#define TGUI_CANVA_CAST(w) TGUI_CONTAINER_OF(w, tgui_canva_t, widget)

tgui_canva_t *tgui_canva_new(void);
void *tgui_canva_get_ctx(tgui_canva_t *canva);
void tgui_canva_set_dirty(tgui_canva_t *canva, long x, long y, long width, long height);

#endif