#ifndef TGUI_IMAGE_H
#define TGUI_IMAGE_H

#include <stddef.h>

typedef struct tgui_image {
	void *private;
	char *filename;
	size_t ref_count;
	long width;
	long height;
} tgui_image_t;

tgui_image_t *tgui_image_new(const char *filename);
tgui_image_t *tgui_image_ref(tgui_image_t *image);
void tgui_image_release(tgui_image_t *images);
long tgui_image_get_width(tgui_image_t *image);
long tgui_image_get_height(tgui_image_t *image);

#endif
