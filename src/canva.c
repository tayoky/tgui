#include <canva.h>
#include <platform.h>

TOBJECT_DEFINE_CLASS(tgui_canva, TGUI_CANVA, tgui_widget_get_type())

static int tgui_canva_destructor(void *object) {
	tgui_canva_t *canva = TGUI_CANVA_CAST(object);
	tgui_platform_canva_destroy(canva);
	return tgui_canva_get_parent_class()->destructor(object);
}

static int tgui_canva_constructor(void *object) {
	tgui_canva_get_parent_class()->constructor(object);
	tgui_canva_t *canva = TGUI_CANVA_CAST(object);
	tgui_platform_canva_create(canva);
	return 0;
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

static void tgui_canva_class_init(tgui_canva_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->allocate_space = tgui_canva_allocate_space;
	widget_class->render         = tgui_canva_render;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_canva_constructor;
	tobject_class->destructor  = tgui_canva_destructor;
}

tgui_canva_t *tgui_canva_new(void) {
	return tobject_new(tgui_canva_get_type());
}

void *tgui_canva_get_ctx(tgui_canva_t *canva) {
	return canva->private;
}

void tgui_canva_set_dirty(tgui_canva_t *canva, long x, long y, long width, long height) {
	tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(canva));
	tgui_rect_t rect;
	tgui_rect_init(&rect, x + TGUI_WIDGET_CAST(canva)->x, y + TGUI_WIDGET_CAST(canva)->y, width, height);
	tgui_surface_invalidate(surface, &rect);
}
