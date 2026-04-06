#include <titlebar.h>
#include <button.h>
#include <label.h>

static tgui_widget_class_t title_bar_class = {
	.size = sizeof(tgui_title_bar_t),
	.name = "title bar",
	.calculate_sizes = tgui_box_calculate_sizes,
	.allocate_space  = tgui_box_allocate_space,
};

tgui_title_bar_t *tgui_title_bar_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&title_bar_class);
	if (!widget) return NULL;

	tgui_title_bar_t *title_bar = TGUI_TITLE_BAR_CAST(widget);
	tgui_widget_set_orientation(widget, TGUI_ORIENTATION_HORIZONTAL);
	title_bar->title = tgui_label_new("tgui window");
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(title_bar->title), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(title_bar->title), TGUI_TRUE);
	tgui_widget_set_halign(TGUI_WIDGET_CAST(title_bar->title), TGUI_ALIGN_LEFT);
	tgui_widget_set_valign(TGUI_WIDGET_CAST(title_bar->title), TGUI_ALIGN_CENTER);
	title_bar->minimize = tgui_button_new();
	tgui_button_set_text(title_bar->minimize, "-");
	title_bar->maximize = tgui_button_new();
	tgui_button_set_text(title_bar->maximize, "[]");
	title_bar->close    = tgui_button_new();
	tgui_button_set_text(title_bar->close, "X");
	tgui_box_append_widget(&title_bar->box, TGUI_WIDGET_CAST(title_bar->title));
	tgui_box_append_widget(&title_bar->box, TGUI_WIDGET_CAST(title_bar->minimize));
	tgui_box_append_widget(&title_bar->box, TGUI_WIDGET_CAST(title_bar->maximize));
	tgui_box_append_widget(&title_bar->box, TGUI_WIDGET_CAST(title_bar->close));
	return title_bar;
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
