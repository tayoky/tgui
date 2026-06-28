#include <stdio.h>
#include <stdlib.h>
#include <tgui/tgui.h>

tgui_window_t *window;

void close_window(void) {
	tgui_quit();
}

void add_element(tgui_box_t *box, tgui_widget_t *widget) {
	char text[256];
	sprintf(text, "this is a %s", tgui_widget_type_from_object(widget)->name);
	tgui_label_t *label = tgui_label_new(text);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(label));
	tgui_widget_set_hexpand(widget, TGUI_TRUE);
	tgui_box_append_widget(box, widget);
}

void add_text(tgui_box_t *box, const char *text) {
	tgui_label_t *label = tgui_label_new(text);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(label));
}

void add_button(tgui_box_t *box, int align) {
	tgui_button_t *button = tgui_button_new();
	tgui_button_set_text(button, "button");
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
	tgui_widget_set_halign(TGUI_WIDGET_CAST(button), align);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(button));
}

tgui_widget_t *input_tab() {
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);

	tgui_scrolled_window_t *main_scrolled = tgui_scrolled_window_new();
	tgui_scrolled_window_set_hpolicy(main_scrolled, TGUI_SCROLLED_WINDOW_POLICY_NEVER);
	tgui_scrolled_window_set_child(main_scrolled, TGUI_WIDGET_CAST(box));

	tgui_button_t *button = tgui_button_new();
	tgui_button_set_text(button, "button");
	add_element(box, TGUI_WIDGET_CAST(button));

	tgui_toggle_button_t *toggle_button = tgui_toggle_button_new();
	tgui_button_set_text(TGUI_BUTTON_CAST(toggle_button), "toggle");
	add_element(box, TGUI_WIDGET_CAST(toggle_button));

	tgui_checkbox_t *checkbox = tgui_checkbox_new();
	add_element(box, TGUI_WIDGET_CAST(checkbox));
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(checkbox), TGUI_FALSE);

	tgui_counter_t *counter = tgui_counter_new();
	add_element(box, TGUI_WIDGET_CAST(counter));

	tgui_slider_t *slider = tgui_slider_new(TGUI_ORIENTATION_HORIZONTAL);
	tgui_slider_set_value(slider, 0.3f);
	add_element(box, TGUI_WIDGET_CAST(slider));

	tgui_scrollbar_t *scrollbar = tgui_scrollbar_new(TGUI_ORIENTATION_HORIZONTAL);
	tgui_scrollbar_set_total_size(scrollbar, 600);
	tgui_scrollbar_set_view_size(scrollbar, 100);
	add_element(box, TGUI_WIDGET_CAST(scrollbar));

	tgui_expander_t *expander = tgui_expander_new();
	tgui_expander_set_title_text(expander, "expander");
	tgui_expander_set_child(expander, TGUI_WIDGET_CAST(tgui_label_new("some really long text...")));
	add_element(box, TGUI_WIDGET_CAST(expander));

	tgui_scrolled_window_t *scrolled_window = tgui_scrolled_window_new();
	tgui_scrolled_window_set_child(scrolled_window, TGUI_WIDGET_CAST(tgui_label_new("some text that is very long and need a scrolled window in order to be read completly")));
	add_element(box, TGUI_WIDGET_CAST(scrolled_window));

	return TGUI_WIDGET_CAST(main_scrolled);
}

tgui_widget_t *align_tab() {
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);

	add_text(box, "left");
	add_button(box, TGUI_ALIGN_LEFT);
	add_text(box, "center");
	add_button(box, TGUI_ALIGN_CENTER);
	add_text(box, "right");
	add_button(box, TGUI_ALIGN_RIGHT);
	add_text(box, "fill");
	add_button(box, TGUI_ALIGN_FILL);

	return TGUI_WIDGET_CAST(box);
}

static tgui_string_list_t *list;
static tgui_counter_t *index;

void insert_element(void) {
	size_t i = tgui_counter_get_value(index);
	tgui_string_list_insert(list, i, "inserted item");
	tgui_counter_set_range(index, 0, tgui_list_model_get_count(TGUI_LIST_MODEL_CAST(list)));
}

void remove_element(void) {
	size_t i = tgui_counter_get_value(index);
	tgui_string_list_remove(list, i);
	tgui_counter_set_range(index, 0, tgui_list_model_get_count(TGUI_LIST_MODEL_CAST(list)));
}

tgui_widget_t *list_tab() {
	tgui_box_t *box = tgui_box_new();

	index = tgui_counter_new();
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(index));
	tgui_button_t *insert = tgui_button_new();
	tgui_button_set_text(insert, "insert element");
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(insert), "click", TCALLBACK_CAST(insert_element), NULL);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(insert));
	tgui_button_t *remove = tgui_button_new();
	tgui_button_set_text(remove, "remove element");
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(remove), "click", TCALLBACK_CAST(remove_element), NULL);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(remove));

	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	const char *strings[] = {
		"item1",
		"item2",
		"item3",
		NULL,
	};
	list = tgui_string_list_new(strings);
	tgui_counter_set_range(index, 0, tgui_list_model_get_count(TGUI_LIST_MODEL_CAST(list)));

	tgui_scrolled_window_t *scrolled_window = tgui_scrolled_window_new();

	tgui_list_view_t *list_view = tgui_list_view_new(tgui_string_factory(), TGUI_LIST_MODEL_CAST(list));
	add_element(box, TGUI_WIDGET_CAST(list_view));
	tgui_scrolled_window_set_child(scrolled_window, TGUI_WIDGET_CAST(list_view));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(scrolled_window));


	return TGUI_WIDGET_CAST(box);
}

tgui_widget_t *menu_tab() {
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(box), TGUI_ORIENTATION_HORIZONTAL);

	tgui_popover_t *file = tgui_popover_new();
	tgui_popover_set_child(file, TGUI_WIDGET_CAST(tgui_label_new("file menu")));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(tgui_submenu_button_new(file, "file")));

	tgui_popover_t *edit = tgui_popover_new();
	tgui_popover_set_child(edit, TGUI_WIDGET_CAST(tgui_label_new("edit menu")));
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(tgui_submenu_button_new(edit, "edit")));

	return TGUI_WIDGET_CAST(box);
}

int main() {
	if (tgui_init() < 0) {
		puts("fail to init twm");
		return 1;
	}

	window = tgui_window_new("tgui demo", 640, 480);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(window), "destroy", TCALLBACK_CAST(close_window), NULL);
	
	tgui_box_t *box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(box), TGUI_TRUE);
	tgui_window_set_child(window, TGUI_WIDGET_CAST(box));

	tgui_stack_switcher_t *tabs = tgui_stack_switcher_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(tabs), TGUI_TRUE);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(tabs));

	tgui_stack_t *stack = tgui_stack_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(stack), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(stack), TGUI_TRUE);
	tgui_stack_switcher_set_stack(tabs, stack);
	tgui_box_append_widget(box, TGUI_WIDGET_CAST(stack));

	tgui_stack_add_child(stack, input_tab(), "inputs");
	tgui_stack_add_child(stack, align_tab(), "aligns");
	tgui_stack_add_child(stack, list_tab(), "lists");
	tgui_stack_add_child(stack, menu_tab(), "menus");
	tgui_main();
	tgui_fini();
	return 0;
}
