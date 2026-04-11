#include <platform.h>
#include <image.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static tgui_list_t images;

tgui_image_t *tgui_image_load(const char *filename) {
	// is it aready in cache ?
	TGUI_LIST_FOREACH(node, &images) {
		tgui_image_t *image = TGUI_IMAGE_CAST(node);
		if (!strcmp(image->filename, filename)) {
			return image;
		}
	}

	tgui_image_t *image = malloc(sizeof(tgui_image_t));
	memset(image, 0, sizeof(tgui_image_t));
	image->filename = strdup(filename);
	if (tgui_platform_load_image(image) < 0) {
		free(image->filename);
		free(image);
		return NULL;
	}
	tgui_list_append(&images, &image->node);
	return image;
}

tgui_image_t *tgui_image_load_icon(const char *icon) {
	// add path to your system's icon here
	const char *icon_path = "/usr/share/icons";
	char full_path[256];
	sprintf(full_path, "%s/%s", icon_path, icon);
	return tgui_image_load(full_path);
}

tgui_image_t *tgui_image_ref(tgui_image_t *image) {
	if (image) image->ref_count++;
	return image;
}

void tgui_image_release(tgui_image_t *image) {
	if (!image) return;
	if (image->ref_count > 1) {
		image->ref_count--;
		return;
	}
	tgui_list_remove(&images, &image->node);
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
