#ifndef TGUI_GRID_H
#define TGUI_GRID_H

#include "widget.h"

typedef struct tgui_grid {
	tgui_widget_t widget;
	unsigned int columns;
	unsigned int rows;
	tgui_widget_t **grid;
	int flags;
} tgui_grid_t;

#define TGUI_GRID_CAST(w) TGUI_CONTAINER_OF(w, tgui_grid_t, widget)

#define TGUI_GRID_SAME_WIDTH  0x01
#define TGUI_GRID_SAME_HEIGHT 0x02

tgui_grid_t *tgui_grid_new(unsigned int columns, unsigned int rows);
void tgui_grid_set_at(tgui_grid_t *grid, unsigned int x, unsigned int y, tgui_widget_t *child);
tgui_widget_t *tgui_grid_get_at(tgui_grid_t *grid, unsigned int x, unsigned int y);
void tgui_grid_set_same_width(tgui_grid_t *grid, int value);
void tgui_grid_set_same_height(tgui_grid_t *grid, int value);

#endif
