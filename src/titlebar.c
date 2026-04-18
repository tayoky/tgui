#include <titlebar.h>
#include <platform.h>
#include <window.h>
#include <button.h>
#include <label.h>

TOBJECT_DEFINE_CLASS(tgui_title_bar, TGUI_TITLE_BAR, tgui_box_get_type())

static int tgui_close_click(tgui_event_t *event) {
	tgui_window_t *window = tgui_widget_get_window(event->widget);
	tgui_widget_destroy(TGUI_WIDGET_CAST(window));
	return TGUI_EVENT_HANDLED;
}

static int tgui_title_bar_click(tgui_event_t *event) {
	tgui_window_t *window = tgui_widget_get_window(event->widget);
	tgui_platform_start_dragging(window, event->click.x, event->click.y);
	return TGUI_EVENT_HANDLED;
}

static int tgui_title_bar_constructor(void *object) {
	tgui_title_bar_get_parent_class()->constructor(object);

	tgui_title_bar_t *title_bar = TGUI_TITLE_BAR_CAST(object);
	tgui_box_t *box = TGUI_BOX_CAST(object);
	tgui_widget_t *widget = TGUI_WIDGET_CAST(object);
	tgui_widget_set_orientation(widget, TGUI_ORIENTATION_HORIZONTAL);
	tgui_widget_set_callback(widget, TGUI_EVENT_CLICK, tgui_title_bar_click, NULL);
	title_bar->title = tgui_label_new("tgui window");
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(title_bar->title), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(title_bar->title), TGUI_TRUE);
	tgui_widget_set_halign(TGUI_WIDGET_CAST(title_bar->title), TGUI_ALIGN_LEFT);
	tgui_widget_set_valign(TGUI_WIDGET_CAST(title_bar->title), TGUI_ALIGN_CENTER);
	title_bar->minimize = tgui_button_new();
	tgui_button_set_icon(title_bar->minimize, "window-minimize");
	title_bar->maximize = tgui_button_new();
	tgui_button_set_icon(title_bar->maximize, "window-maximize");
	title_bar->close    = tgui_button_new();
	tgui_button_set_icon(title_bar->close, "window-close");
	tgui_widget_set_callback(TGUI_WIDGET_CAST(title_bar->close), TGUI_EVENT_CLICK, tgui_close_click, NULL);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(title_bar->title));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(title_bar->minimize));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(title_bar->maximize));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(title_bar->close));
	return 0;
}

static void tgui_title_bar_class_init(tgui_title_bar_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_title_bar_constructor;
}

tgui_title_bar_t *tgui_title_bar_new(void) {
	return tobject_new(tgui_title_bar_get_type());
}

void tgui_title_bar_set_minimizable(tgui_title_bar_t *bar, int enabled) {
	tgui_widget_set_visible(TGUI_WIDGET_CAST(bar->minimize), enabled);
}

void tgui_title_bar_set_maximizable(tgui_title_bar_t *bar, int enabled) {
	tgui_widget_set_visible(TGUI_WIDGET_CAST(bar->minimize), enabled);
}

void tgui_title_bar_set_closable(tgui_title_bar_t *bar, int enabled) {
	tgui_widget_set_visible(TGUI_WIDGET_CAST(bar->minimize), enabled);
}

void tgui_title_bar_set_title(tgui_title_bar_t *bar, const char *title) {
	tgui_label_set_text(bar->title, title);
}
