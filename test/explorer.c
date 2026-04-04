#include <stdio.h>
#include <tgui/tgui.h>

tgui_paned_t *main_paned;
tgui_window_t *window;
tgui_box_t *sidebar;
tgui_box_t *directory_box;

int main() {
	if (tgui_init() < 0) {
		puts("failed to init tgui");
		return 1;
	}
	window = tgui_window_new("tgui window", 640, 480);
	tgui_window_set_scaling(window, 3);
	main_paned = tgui_paned_new(TGUI_ORIENTATION_HORIZONTAL);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(main_paned), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(main_paned), TGUI_TRUE);

	sidebar = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(sidebar), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(sidebar), TGUI_TRUE);

	directory_box = tgui_box_new();
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(directory_box), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(directory_box), TGUI_TRUE);


	// setup the hierarchy
	tgui_paned_set_first(main_paned, TGUI_WIDGET_CAST(sidebar));
	tgui_paned_set_last(main_paned, TGUI_WIDGET_CAST(directory_box));
	tgui_window_set_child(window, TGUI_WIDGET_CAST(main_paned));

	
	tgui_main();
	tgui_fini();
	return 0;
}
