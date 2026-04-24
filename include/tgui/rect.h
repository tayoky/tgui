#ifndef TGUI_RECT_H
#define TGUI_RECT_H



typedef struct tgui_rect {
	long start_x;
	long start_y;
	long end_x;
	long end_y;
} tgui_rect_t;

void tgui_rect_init(tgui_rect_t *rect, long x, long y, long width, long height);

/**
 * @brief calculate the rect intersection of two other rect
 * @param dest where to put the new rect
 * @param a the first rect
 * @param b the second rect
 */
void tgui_rect_intersect(tgui_rect_t *dest, tgui_rect_t *a, tgui_rect_t *b);

int tgui_rect_collide(tgui_rect_t *a, tgui_rect_t *b);

#endif
