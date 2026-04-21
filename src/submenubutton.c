#include <submenubutton.h>

TOBJECT_DEFINE_CLASS(tgui_submenu_button, TGUI_SUBMENU_BUTTON, tgui_popover_button_get_type())

static void tgui_submenu_button_class_init(tgui_submenu_button_class_t *class) {
	(void)class;
}

tgui_popover_button_t *tgui_submenu_button_new(tgui_popover_t *popover, const char *name) {
	tgui_submenu_button_t *submenu_button = tobject_new(tgui_submenu_button_get_type());
	if (!submenu_button) return NULL;

	tgui_popover_button_set_popover(TGUI_POPOVER_BUTTON_CAST(submenu_button), popover);
	tgui_button_set_text(TGUI_BUTTON_CAST(submenu_button), name);
	return TGUI_POPOVER_BUTTON_CAST(submenu_button);
}
