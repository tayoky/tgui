#include <rect.h>
#include <common.h>

void tgui_rect_init(tgui_rect_t *rect, long x, long y, long width, long height) {
	rect->start_x = x;
	rect->start_y = y;
	rect->end_x = x + width;
	rect->end_y = y + height;
}

void tgui_rect_intersect(tgui_rect_t *dest, tgui_rect_t *a, tgui_rect_t *b) {
	dest->start_x = max(a->start_x, b->start_x);
	dest->start_y = max(a->start_y, b->start_y);
	dest->end_x = min(a->end_x, b->end_x);
	dest->end_y = min(a->end_y, b->end_y);
}

int tgui_rect_collide(tgui_rect_t *a, tgui_rect_t *b) {
	if (a->start_x >= b->end_x) return 0;
	if (a->end_x <= b->start_x) return 0;
	if (a->start_y >= b->end_y) return 0;
	if (a->end_y <= b->start_y) return 0;
	return 1;
}
