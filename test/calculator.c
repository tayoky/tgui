#include <stdio.h>
#include <tgui/tgui.h>
	
tgui_button_t *new_button(const char *text) {
	tgui_button_t *button = tgui_button_new();
	tgui_widget_set_margin(TGUI_WIDGET_CAST(button), 1);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
	tgui_button_set_text(button, text);
	return button;
}

int main() {
	if (tgui_init() < 0) {
		puts("fail to init tgui");
		return -1;
	}

	tgui_window_t *window = tgui_window_new("tgui calculator", 200, 200);

	// create the grid with all the buttons
	tgui_grid_t *grid = tgui_grid_new(4, 4);
	for (int i=1; i<10; i++) {
		int x = (i - 1) % 3;
		int y = 2 - ((i - 1)/ 3);

		char text[10];
		sprintf(text, "%d", i);
		tgui_button_t *button = new_button(text);
		tgui_grid_set_at(grid, x, y, TGUI_WIDGET_CAST(button));
	}
	tgui_grid_set_at(grid, 1, 3, TGUI_WIDGET_CAST(new_button("0")));
	tgui_grid_set_at(grid, 3, 0, TGUI_WIDGET_CAST(new_button("/")));
	tgui_grid_set_at(grid, 3, 1, TGUI_WIDGET_CAST(new_button("*")));
	tgui_grid_set_at(grid, 3, 2, TGUI_WIDGET_CAST(new_button("-")));
	tgui_grid_set_at(grid, 3, 3, TGUI_WIDGET_CAST(new_button("+")));

	tgui_window_set_child(window, TGUI_WIDGET_CAST(grid));

	tgui_main();
	tgui_fini();
}
