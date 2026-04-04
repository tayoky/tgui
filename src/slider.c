#include <slider.h>
#include <button.h>

static void tgui_slider_calculate_sizes(tgui_widget_t *widget) {
	tgui_slider_t *slider = TGUI_SLIDER_CAST(widget);

	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(slider->button));

	long button_min_lenght;
	long button_pref_lenght;
	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		button_min_lenght  = slider->button->widget.min_height;
		button_pref_lenght = slider->button->widget.pref_height;
	} else {
		button_min_lenght  = slider->button->widget.min_width;
		button_pref_lenght = slider->button->widget.pref_width;
	}

	long slider_min_lenght;
	long slider_pref_lenght;
	if (slider->size == TGUI_SLIDER_SIZE_AUTO) {
		// the slider must be at least two times the size of the button
		slider_min_lenght  = button_min_lenght * 2;
		slider_pref_lenght = button_pref_lenght * 2;
	} else {
		// we know how big is the slider
		// and the portion of the slider it take
		// we can calculate how big is the slider
		slider_min_lenght  = button_min_lenght / slider->size;
		slider_pref_lenght = button_pref_lenght / slider->size;
	}

	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		widget->min_width  = slider->button->widget.min_width;
		widget->pref_width = slider->button->widget.pref_width;
		widget->min_height  = slider_min_lenght;
		widget->pref_height = slider_pref_lenght;
	} else {
		widget->min_width  = slider_min_lenght;
		widget->pref_width = slider_pref_lenght;
		widget->min_height  = slider->button->widget.min_height;
		widget->pref_height = slider->button->widget.pref_height;
	}
}

static void tgui_slider_allocate_space(tgui_widget_t *widget) {
	tgui_slider_t *slider = TGUI_SLIDER_CAST(widget);
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);

	// put the value on a 0 to 1 scale
	// for easier calcul
	double val = (slider->value - slider->min) / (slider->max - slider->min);

	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		long button_height;
		if (slider->size == TGUI_SLIDER_SIZE_AUTO) {
			button_height = height / 2;
			if (button_height > slider->button->widget.pref_height) {
				button_height = slider->button->widget.pref_height;
			}
		} else {
			button_height = height * slider->size;
		}
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(slider->button), x, y + (height - button_height) * val, width, button_height);
	} else {
		long button_width;
		if (slider->size == TGUI_SLIDER_SIZE_AUTO) {
			button_width = width / 2;
			if (button_width > slider->button->widget.pref_width) {
				button_width = slider->button->widget.pref_width;
			}
		} else {
			button_width = width * slider->size;
		}
		tgui_widget_allocate_space(TGUI_WIDGET_CAST(slider->button), x + (width - button_width) * val, y, button_width, height);
	}
}

static tgui_widget_class_t slider_class = {
	.size = sizeof(tgui_slider_t),
	.name = "slider",
	.calculate_sizes = tgui_slider_calculate_sizes,
	.allocate_space  = tgui_slider_allocate_space
};

tgui_slider_t *tgui_slider_new(int orientation) {
	tgui_widget_t *widget = tgui_widget_new(&slider_class);
	if (!widget) return NULL;

	tgui_slider_t *slider = TGUI_SLIDER_CAST(widget);
	tgui_widget_set_orientation(widget, orientation);
	slider->min  = 0;
	slider->max  = 1;
	slider->size = TGUI_SLIDER_SIZE_AUTO;
	slider->button = tgui_button_new();
	tgui_widget_set_parent(TGUI_WIDGET_CAST(slider->button), TGUI_WIDGET_CAST(slider));
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(slider->button), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(slider->button), TGUI_TRUE);
	tgui_slider_set_text(slider, "-");
	return slider;
}

static void tgui_slider_clamp_value(tgui_slider_t *slider) {
	if (slider->value < slider->min) {
		slider->value = slider->min;
	} else if (slider->value > slider->max) {
		slider->value = slider->max;
	}
}

void tgui_slider_set_range(tgui_slider_t *slider, double min, double max) {
	slider->min = min;
	slider->max = max;
	tgui_slider_clamp_value(slider);
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(slider->button));
}

void tgui_slider_set_value(tgui_slider_t *slider, double value) {
	if (slider->value == value) return;
	slider->value = value;
	tgui_slider_clamp_value(slider);
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(slider->button));
}

void tgui_slider_set_size(tgui_slider_t *slider, double size) {
	slider->size = size;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(slider->button));
}

void tgui_slider_set_text(tgui_slider_t *slider, const char *text) {
	tgui_button_set_text(slider->button, text);
}
void tgui_slider_set_icon(tgui_slider_t *slider, const char *icon) {
	tgui_button_set_icon(slider->button, icon);
}

double tgui_slider_get_value(tgui_slider_t *slider) {
	return slider->value;
}
