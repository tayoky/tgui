#include <stdlib.h>
#include <string.h>
#include <widget.h>
#include <grid.h>

static void tgui_grid_calculate_column_sizes(tgui_grid_t *grid, unsigned int x, long *col_min_width, long *col_pref_width) {
	*col_min_width  = 0;
	*col_pref_width = 0;
	for (unsigned int y=0; y<grid->rows; y++) {
		tgui_widget_t *child = tgui_grid_get_at(grid, x, y);
		if (!child) continue;
		tgui_widget_calculate_sizes(child);
		if (child->min_width > *col_min_width) *col_min_width = child->min_width;
		if (child->pref_width > *col_pref_width) *col_pref_width = child->pref_width;
	}
}

static void tgui_grid_calculate_row_sizes(tgui_grid_t *grid, unsigned int y, long *row_min_height, long *row_pref_height) {
	*row_min_height  = 0;
	*row_pref_height = 0;
	for (unsigned int x=0; x<grid->columns; x++) {
		tgui_widget_t *child = tgui_grid_get_at(grid, x, y);
		if (!child) continue;
		tgui_widget_calculate_sizes(child);
		if (child->min_height > *row_min_height) *row_min_height = child->min_height;
		if (child->pref_height > *row_pref_height) *row_pref_height = child->pref_height;
	}
}

static void tgui_grid_calculate_sizes(tgui_widget_t *widget) {
	tgui_grid_t *grid = TGUI_GRID_CAST(widget);

	// calculate the width of each columns
	long min_width  = 0;
	long pref_width = 0;
	for (unsigned int x=0; x<grid->columns; x++) {
		long col_min_width;
		long col_pref_width;
		tgui_grid_calculate_column_sizes(grid, x, &col_min_width, &col_pref_width);
		min_width  += col_min_width;
		pref_width += col_pref_width;
	}

	// calculate the height of each columns
	long min_height  = 0;
	long pref_height = 0;
	for (unsigned int y=0; y<grid->rows; y++) {
		long row_min_height;
		long row_pref_height;
		tgui_grid_calculate_row_sizes(grid, y, &row_min_height, &row_pref_height);
		min_height  += row_min_height;
		pref_height += row_pref_height;
	}
	grid->widget.min_width  = min_width;
	grid->widget.min_height = min_height;
	grid->widget.pref_width  = pref_width;
	grid->widget.pref_height = pref_height;
}
static void tgui_grid_allocate_space(tgui_widget_t *widget) {
	tgui_grid_t *grid = TGUI_GRID_CAST(widget);
	long width  = tgui_widget_get_outer_width(widget);
	long height = tgui_widget_get_outer_height(widget);
	long width_excess  = width - widget->min_width;
	long height_excess = height - widget->min_height;
	long pref_width_total = widget->pref_width - widget->min_width;
	long pref_height_total = widget->pref_height - widget->min_height;

	long widths[grid->columns];
	for (unsigned int x=0; x<grid->columns; x++) {
		long col_min_width;
		long col_pref_width;
		tgui_grid_calculate_column_sizes(grid, x, &col_min_width, &col_pref_width);
		col_pref_width -= col_min_width;
		long col_width = col_min_width;
		if (pref_width_total) {
			col_width += col_pref_width * width_excess / pref_width_total;
		}
		widths[x] = col_width;
	}

	long heights[grid->rows];
	for (unsigned int y=0; y<grid->rows; y++) {
		long row_min_height;
		long row_pref_height;
		tgui_grid_calculate_row_sizes(grid, y, &row_min_height, &row_pref_height);
		row_pref_height -= row_min_height;
		long row_height = row_min_height;
		if (pref_height_total) {
			row_height += row_pref_height * height_excess / pref_height_total;
		}
		heights[y] = row_height;
	}

	
	long p_x = tgui_widget_get_inner_x(widget);
	for (unsigned int x=0; x<grid->columns; x++) {
		long p_y = tgui_widget_get_inner_y(widget);
		long col_width = widths[x];
		if (grid->flags & TGUI_GRID_SAME_WIDTH) {
			col_width = tgui_widget_get_inner_width(widget) / grid->columns;
		}
		for (unsigned int y=0; y<grid->rows; y++) {
			long row_height = heights[y];
			if (grid->flags & TGUI_GRID_SAME_HEIGHT) {
				row_height = tgui_widget_get_inner_height(widget) / grid->rows;
			}
			tgui_widget_t *child = tgui_grid_get_at(grid, x, y);
			if (child) {
				tgui_widget_allocate_space(child, p_x, p_y, col_width, row_height);
			}
			p_y += row_height;
		}
		p_x += col_width;
	}
}

static tgui_widget_class_t grid_class = {
	.size = sizeof(tgui_grid_t),
	.name = "grid",
	.calculate_sizes = tgui_grid_calculate_sizes,
	.allocate_space  = tgui_grid_allocate_space,
};

tgui_grid_t *tgui_grid_new(unsigned int columns, unsigned int rows) {
	tgui_widget_t *widget = tgui_widget_new(&grid_class);
	if (!widget) return NULL;

	tgui_grid_t *grid = TGUI_GRID_CAST(widget);
	grid->columns = columns;
	grid->rows    = rows;
	grid->grid = malloc(sizeof(tgui_widget_t*) * columns * rows);
	memset(grid->grid, 0, sizeof(tgui_widget_t*) * columns * rows);
	return grid;
}

void tgui_grid_set_at(tgui_grid_t *grid, unsigned int x, unsigned int y, tgui_widget_t *child) {
	// can only have one child per cell
	tgui_widget_destroy(grid->grid[x + y * grid->columns]);
	if (child) {
		tgui_widget_set_parent(child, TGUI_WIDGET_CAST(grid));
		grid->grid[x + y * grid->columns] = child;
	}
}

tgui_widget_t *tgui_grid_get_at(tgui_grid_t *grid, unsigned int x, unsigned int y) {
		return grid->grid[x + y * grid->columns];
}

void tgui_grid_set_same_width(tgui_grid_t *grid, int value) {
	if (value) {
		grid->flags |= TGUI_GRID_SAME_WIDTH;
	} else {
		grid->flags &= ~TGUI_GRID_SAME_WIDTH;
	}
}

void tgui_grid_set_same_height(tgui_grid_t *grid, int value) {
	if (value) {
		grid->flags |= TGUI_GRID_SAME_HEIGHT;
	} else {
		grid->flags &= ~TGUI_GRID_SAME_HEIGHT;
	}
}
