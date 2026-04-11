#include <canva.h>
#include <platform.h>

static void tgui_canva_free(tgui_widget_t *widget) {
    tgui_canva_t *canva = TGUI_CANVA_CAST(widget);
    tgui_platform_canva_destroy(canva);
}

static void tgui_canva_allocate_space(tgui_widget_t *widget) {
    tgui_canva_t *canva = TGUI_CANVA_CAST(widget);

    // the canva reszied we need to recreate the context
    tgui_platform_canva_destroy(canva);
    tgui_platform_canva_create(canva);
}

static void tgui_canva_render(tgui_widget_t *widget) {
    tgui_canva_t *canva = TGUI_CANVA_CAST(widget);
    tgui_platform_push_canva(canva);
}

static tgui_widget_class_t canva_class  = {
    .size = sizeof(tgui_canva_t),
    .name = "canva",
    .free = tgui_canva_free,
    .allocate_space = tgui_canva_allocate_space,
    .render         = tgui_canva_render,
};

tgui_canva_t *tgui_canva_new(void) {
    tgui_widget_t *widget = tgui_widget_new(&canva_class);
    if (!widget) return NULL;
    
    tgui_canva_t *canva = TGUI_CANVA_CAST(widget);
    tgui_platform_canva_create(canva);
    return canva;
}

void *tgui_canva_get_ctx(tgui_canva_t *canva) {
    return canva->private;
}

void tgui_canva_set_dirty(tgui_canva_t *canva, long x, long y, long width, long height) {
    tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(canva));
    tgui_surface_invalidate(surface, canva->widget.x + x, canva->widget.y + y, width, height);
}
