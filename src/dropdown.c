#include <dropdown.h>

TOBJECT_DEFINE_CLASS(tgui_dropdown, TGUI_DROPDOWN, tgui_popover_button_get_type())

static int tgui_dropdown_constructor(void *object) {
	tgui_dropdown_get_parent_class()->constructor(object);

	tgui_dropdown_t *dropdown = TGUI_DROPDOWN_CAST(object);
	dropdown->popover = tgui_popover_new();
	dropdown->list_view = tgui_list_view_new(NULL, NULL);
	tgui_popover_set_child(dropdown->popover, TGUI_WIDGET_CAST(dropdown->list_view));
	tgui_popover_button_set_popover(TGUI_POPOVER_BUTTON_CAST(object), dropdown->popover);
	return 0;
}

static int tgui_dropdown_destructor(void *object) {
	tgui_dropdown_t *dropdown = TGUI_DROPDOWN_CAST(object);
	tgui_widget_destroy(TGUI_WIDGET_CAST(dropdown->popover));

	return tgui_dropdown_get_parent_class()->destructor(object);
}

static void tgui_dropdown_class_init(tgui_dropdown_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_dropdown_constructor;
	tobject_class->destructor  = tgui_dropdown_destructor;
}

tgui_dropdown_t *tgui_dropdown_new(tgui_factory_t *factory, tgui_list_t *list) {
	tgui_dropdown_t *dropdown = tobject_new(tgui_dropdown_get_type());
	if (!dropdown) return NULL;

	tgui_dropdown_set_factory(dropdown, factory);
	tgui_dropdown_set_list(dropdown, list);
	return dropdown;
}

void tgui_dropdown_set_factory(tgui_dropdown_t *dropdown, tgui_factory_t *factory) {
	tgui_list_view_set_factory(dropdown->list_view, factory);
}

void tgui_dropdown_set_list(tgui_dropdown_t *dropdown, tgui_list_t *list) {
	tgui_list_view_set_list(dropdown->list_view, list);
}

void tgui_dropdown_set_selected(tgui_dropdown_t *dropdown, void *selected);

void *tgui_dropdown_get_selected(tgui_dropdown_t *dropdown) {
	return dropdown->selected;
}
