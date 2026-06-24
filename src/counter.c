#include <counter.h>
#include <label.h>
#include <button.h>
#include <box.h>
#include <limits.h>
#include <stdio.h>

TOBJECT_DEFINE_CLASS(tgui_counter, TGUI_COUNTER, tgui_box_get_type())

static void tgui_counter_minus_click(tgui_button_t *minus, int stub, tgui_counter_t *counter) {
	(void)minus;
	(void)stub;
	tgui_counter_set_value(counter, counter->value - counter->incr);
}

static void tgui_counter_plus_click(tgui_button_t *plus, int stub, tgui_counter_t *counter) {
	(void)plus;
	(void)stub;
	tgui_counter_set_value(counter, counter->value + counter->incr);
}

static int tgui_counter_constructor(void *object) {
	tgui_counter_get_parent_class()->constructor(object);

	tgui_counter_t *counter = TGUI_COUNTER_CAST(object);
	counter->min = 0;
	counter->max = LONG_MAX;
	counter->incr = 1;
	// TODO : text input ???
	counter->label = tgui_label_new("0");
	counter->minus = tgui_button_new();
	counter->plus  = tgui_button_new();
	tgui_button_set_text(counter->minus, "-");
	tgui_button_set_text(counter->plus , "+");
	tgui_box_append_widget(TGUI_BOX_CAST(counter), TGUI_WIDGET_CAST(counter->label));
	tgui_box_append_widget(TGUI_BOX_CAST(counter), TGUI_WIDGET_CAST(counter->minus));
	tgui_box_append_widget(TGUI_BOX_CAST(counter), TGUI_WIDGET_CAST(counter->plus));
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(counter), TGUI_ORIENTATION_HORIZONTAL);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(counter->label), TGUI_TRUE);
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(counter->label), TGUI_TRUE);
	tgui_widget_set_valign(TGUI_WIDGET_CAST(counter->label), TGUI_ALIGN_CENTER);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(counter->minus), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(counter->plus), TGUI_TRUE);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(counter->minus), "click", TCALLBACK_CAST(tgui_counter_minus_click), counter);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(counter->plus), "click", TCALLBACK_CAST(tgui_counter_plus_click), counter);
	return 0;
}

static void tgui_counter_class_init(tgui_counter_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_counter_constructor;
}

tgui_counter_t *tgui_counter_new(void) {
	return tobject_new(tgui_counter_get_type());
}

void tgui_counter_set_value(tgui_counter_t *counter, long value) {
	if (value > counter->max) {
		value = counter->max;
	}
	if (value < counter->min) {
		value = counter->min;
	}
	if (counter->value == value) {
		return;
	}
	counter->value = value;
	char buf[64];
	snprintf(buf, sizeof(buf), "%ld", value);
	tgui_label_set_text(counter->label, buf);
	tgui_widget_send_signal(TGUI_WIDGET_CAST(counter), "changed", &value);
}

long tgui_counter_get_value(tgui_counter_t *counter) {
	return counter->value;
}

void tgui_counter_set_range(tgui_counter_t *counter, long min, long max) {
	counter->min = min;
	counter->max = max;
}

void tgui_counter_set_increment(tgui_counter_t *counter, long incr) {
	counter->incr = incr;
}
