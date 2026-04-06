#include <widget.h>
#include <icon.h>

static void tgui_icon_calculate_sizes(tgui_widget_t *widget) {
	tgui_icon_t *icon = TGUI_ICON_CAST(widget);
	if (!icon->image) return;
	widget->min_width  = tgui_image_get_width(icon->image);
	widget->min_height = tgui_image_get_height(icon->image);
	widget->pref_width  = tgui_image_get_width(icon->image);
	widget->pref_height = tgui_image_get_height(icon->image);
}

static void tgui_icon_render(tgui_widget_t *widget) {
	tgui_icon_t *icon = TGUI_ICON_CAST(widget);
	if (!icon->image) return;
}

static tgui_widget_class_t icon_class = {
	.size = sizeof(tgui_icon_t),
	.name = "icon",
	.calculate_sizes = tgui_icon_calculate_sizes,
	.render          = tgui_icon_render,
};

tgui_icon_t *tgui_icon_new(const char *name) {
	tgui_widget_t *widget = tgui_widget_new(&icon_class);
	if (!widget) return NULL;

	tgui_icon_t *icon = TGUI_ICON_CAST(widget);
	tgui_icon_set(icon, name);
	return icon;
}

void tgui_icon_set(tgui_icon_t *icon, const char *name) {
	tgui_image_t *image = NULL;
	if (name) {
		tgui_image_load_icon(name);
		if (!image) {
			image = tgui_image_load_icon("image-missing");
			if (!image) return;
		}
	}
	tgui_icon_set_image(icon, image);
}

void tgui_icon_set_image(tgui_icon_t *icon, tgui_image_t *image) {
	tgui_image_release(icon->image);
	icon->image = tgui_image_ref(image);
	tgui_widget_mark_dirty(TGUI_WIDGET_CAST(icon));
	tgui_widget_mark_dirty_size(TGUI_WIDGET_CAST(icon));
}

tgui_image_t *tgui_icon_get_image(tgui_icon_t *icon) {
	return icon->image;
}
