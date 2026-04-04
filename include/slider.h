#ifndef TGUI_SLIDER_H
#define TGUI_SLIDER_H

#include "widget.h"
#include "button.h"

typedef struct tgui_slider {
	tgui_widget_t widget;
	tgui_button_t *button;
	double min;
	double max;
	double value;
	double size;
} tgui_slider_t;

#define TGUI_SLIDER_CAST(w) TGUI_CONTAINER_OF(w, tgui_slider_t, widget)
#define TGUI_SLIDER_SIZE_AUTO -1

tgui_slider_t *tgui_slider_new(int orientation);
void tgui_slider_set_range(tgui_slider_t *slider, double min, double max);
void tgui_slider_set_value(tgui_slider_t *slider, double value);
void tgui_slider_set_size(tgui_slider_t *slider, double size);
void tgui_slider_set_text(tgui_slider_t *slider, const char *text);
void tgui_slider_set_icon(tgui_slider_t *slider, const char *icon);
double tgui_slider_get_value(tgui_slider_t *slider);

#endif
