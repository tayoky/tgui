#include <slider.h>
#include <button.h>

TOBJECT_DEFINE_CLASS(tgui_slider, TGUI_SLIDER, tgui_widget_get_type())

static void tgui_slider_calculate_sizes(tgui_widget_t *widget) {
	tgui_slider_t *slider = TGUI_SLIDER_CAST(widget);

	tgui_widget_calculate_sizes(TGUI_WIDGET_CAST(slider->button));

	long button_min_length;
	long button_pref_length;
	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		button_min_length  = slider->button->widget.min_height;
		button_pref_length = slider->button->widget.pref_height;
	} else {
		button_min_length  = slider->button->widget.min_width;
		button_pref_length = slider->button->widget.pref_width;
	}

	long slider_min_length;
	long slider_pref_length;
	if (slider->size == TGUI_SLIDER_SIZE_AUTO) {
		// the slider must be at least two times the size of the button
		slider_min_length  = button_min_length * 2;
		slider_pref_length = button_pref_length * 2;
	} else {
		// we know how big is the slider
		// and the portion of the slider it take
		// we can calculate how big is the slider
		slider_min_length  = button_min_length / slider->size;
		slider_pref_length = button_pref_length / slider->size;
	}

	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		widget->min_width  = slider->button->widget.min_width;
		widget->pref_width = slider->button->widget.pref_width;
		widget->min_height  = slider_min_length;
		widget->pref_height = slider_pref_length;
	} else {
		widget->min_width  = slider_min_length;
		widget->pref_width = slider_pref_length;
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

static void tgui_slider_button_click(tobject_t *tobject, tgui_event_click_t *event) {
	tgui_slider_t *slider = TGUI_SLIDER_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	tgui_button_t *button = TGUI_BUTTON_CAST(tobject);
	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		slider->offset = button->widget.y - event->y;
	} else {
		slider->offset = button->widget.x - event->x;
	}
}

static void tgui_slider_button_move(tobject_t *tobject, tgui_event_move_t *event) {
	tgui_slider_t *slider = TGUI_SLIDER_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	tgui_button_t *button = TGUI_BUTTON_CAST(tobject);
	if (!event->is_pressed) return;

	long offset;
	long length;
	if (slider->widget.orientation == TGUI_ORIENTATION_VERTICAL) {
		offset = event->abs_y - slider->widget.y + slider->offset;
		length = slider->widget.height - button->widget.height;
	} else {
		offset = event->abs_x - slider->widget.x + slider->offset;
		length = slider->widget.width - button->widget.width;
	}

	double val = (double)offset/(double)length;
	tgui_slider_set_value(slider, slider->min + val * (slider->max - slider->min));
}

static int tgui_slider_constructor(void *object) {
	tgui_slider_get_parent_class()->constructor(object);

	tgui_slider_t *slider = TGUI_SLIDER_CAST(object);
	slider->min  = 0;
	slider->max  = 1;
	slider->size = TGUI_SLIDER_SIZE_AUTO;
	slider->button = tgui_button_new();
	tgui_widget_set_parent(TGUI_WIDGET_CAST(slider->button), TGUI_WIDGET_CAST(slider));
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(slider->button), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(slider->button), TGUI_TRUE);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(slider->button), "click", TCALLBACK_CAST(tgui_slider_button_click), NULL);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(slider->button), "move", TCALLBACK_CAST(tgui_slider_button_move), NULL);
	tgui_slider_set_text(slider, "-");
	// we want the "-" in the middle
	tgui_widget_set_halign(tgui_button_get_child(slider->button), TGUI_ALIGN_CENTER);
	tgui_widget_set_valign(tgui_button_get_child(slider->button), TGUI_ALIGN_CENTER);
	return 0;
}

static void tgui_slider_class_init(tgui_slider_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_slider_calculate_sizes;
	widget_class->allocate_space  = tgui_slider_allocate_space;

	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_slider_constructor;
}

tgui_slider_t *tgui_slider_new(int orientation) {
	tgui_slider_t *slider = tobject_new(tgui_slider_get_type());
	if (!slider) return NULL;
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(slider), orientation);
	return slider;
}

static double tgui_slider_clamp_value(tgui_slider_t *slider, double value) {
	if (value < slider->min) {
		return slider->min;
	} else if (value > slider->max) {
		return slider->max;
	} else {
		return value;
	}
}

void tgui_slider_set_range(tgui_slider_t *slider, double min, double max) {
	if (slider->min == min && slider->max == max) return;
	slider->min = min;
	slider->max = max;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(slider->button));
	// reset the same value to clamp
	tgui_slider_set_value(slider, slider->value);
}

void tgui_slider_set_value(tgui_slider_t *slider, double value) {
	value = tgui_slider_clamp_value(slider, value);
	if (slider->value == value) return;
	slider->value = value;
	tgui_widget_mark_dirty_space(TGUI_WIDGET_CAST(slider->button));
	tgui_widget_send_signal(TGUI_WIDGET_CAST(slider), "changed", &value);
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
