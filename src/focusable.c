#include <focusable.h>
#include <surface.h>

TOBJECT_DEFINE_CLASS(tgui_focusable, TGUI_FOCUSABLE, tgui_widget_get_type())

static void tgui_focusable_click(tgui_focusable_t *focusable) {
	tgui_surface_t *surface = tgui_widget_get_surface(TGUI_WIDGET_CAST(focusable));
	tgui_surface_set_focus(surface, TGUI_WIDGET_CAST(focusable));
}

static int tgui_focusable_constructor(void *object) {
	tgui_focusable_get_parent_class()->constructor(object);

	tgui_focusable_t *focusable = TGUI_FOCUSABLE_CAST(object);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(focusable), "click", TCALLBACK_CAST(tgui_focusable_click), NULL);
	return 0;
}

static void tgui_focusable_class_init(tgui_focusable_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor     = tgui_focusable_constructor;
}
