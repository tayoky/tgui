#include <platform.h>
#include <image.h>
#include <stdlib.h>
#include <string.h>

tgui_image_t *tgui_image_new(const char *filename) {
	tgui_image_t *image = malloc(sizeof(tgui_image_t));
	memset(image, 0, sizeof(tgui_image_t));
	image->filename = strdup(filename);
	if (tgui_platform_load_image(image) < 0) {
		free(image->filename);
		free(image);
		return NULL;
	}
	return image;
}

tgui_image_t *tgui_image_ref(tgui_image_t *image) {
	if (image) image->ref_count++;
	return image;
}

void tgui_image_release(tgui_image_t *image) {
	if (image->ref_count > 1) {
		image->ref_count--;
		return;
	}
	tgui_platform_free_image(image);
	free(image->filename);
	free(image);
}

long tgui_image_get_width(tgui_image_t *image) {
	return image->width;
}

long tgui_image_get_height(tgui_image_t *image) {
	return image->height;
}
