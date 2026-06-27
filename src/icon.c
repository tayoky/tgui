#include <widget.h>
#include <platform.h>
#include <icon.h>

TOBJECT_DEFINE_CLASS(tgui_icon, TGUI_ICON, tgui_widget_get_type())

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
	tgui_surface_t *surface = tgui_widget_get_surface(widget);
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	tgui_platform_render_image(surface, x, y, icon->image);
}

static void tgui_icon_class_init(tgui_icon_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_icon_calculate_sizes;
	widget_class->render          = tgui_icon_render;
}

tgui_icon_t *tgui_icon_new(const char *name) {
	tgui_icon_t *icon = tobject_new(tgui_icon_get_type());
	if (!icon) return NULL;
	tgui_icon_set(icon, name);
	return icon;
}

void tgui_icon_set(tgui_icon_t *icon, const char *name) {
	tgui_image_t *image = NULL;
	if (name) {
		image = tgui_image_load_icon(name);
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
