#include <stringfactory.h>
#include <stringlist.h>
#include <factory.h>
#include <label.h>

static int tgui_string_factory_setup(tgui_factory_t *factory, tgui_list_item_t *item) {
	(void)factory;
	tgui_label_t *label = tgui_label_new("");
	tgui_list_item_set_child(item, TGUI_WIDGET_CAST(label));
	return 0;
}

static int tgui_string_factory_bind(tgui_factory_t *factory, tgui_list_item_t *item) {
	(void)factory;
	tgui_string_item_t *string_item = TGUI_STRING_ITEM_CAST(item->item);
	tgui_label_t *label = TGUI_LABEL_CAST(tgui_list_item_get_child(item));
	tgui_label_set_text(label, string_item->str);
	return 0;
}

static tgui_factory_t string_factory = {
	.setup = tgui_string_factory_setup,
	.bind  = tgui_string_factory_bind,
};

tgui_factory_t *tgui_string_factory(void) {
	return &string_factory;
}
