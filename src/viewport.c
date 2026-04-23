#include <viewport.h>

TOBJECT_DEFINE_CLASS(tgui_viewport, TGUI_VIEWPORT, tgui_widget_get_type())

static void tgui_viewport_class_init(tgui_viewport_class_t *class) {
	(void)class;
}

tgui_viewport_t *tgui_viewport_new(void) {
	return tobject_new(tgui_viewport_get_type());
}

void tgui_viewport_set_scroll_x(tgui_viewport_t *viewport, long scroll_x);
void tgui_viewport_set_scroll_y(tgui_viewport_t *viewport, long scroll_y);
void tgui_viewport_set_have_hscroll(tgui_viewport_t *viewport, char have_hscroll);
void tgui_viewport_set_have_vscroll(tgui_viewport_t *viewport, char have_vscroll);
long tgui_viewport_get_scroll_x(tgui_viewport_t *viewport);
long tgui_viewport_get_scroll_y(tgui_viewport_t *viewport);
char tgui_viewport_get_have_hscroll(tgui_viewport_t *viewport);
char tgui_viewport_get_have_vscroll(tgui_viewport_t *viewport);
long tgui_viewport_get_content_pref_width(tgui_viewport_t *viewport);
long tgui_viewport_get_content_pref_height(tgui_viewport_t *viewport);
