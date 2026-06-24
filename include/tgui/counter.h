#ifndef TGUI_COUNTER_H
#define TGUI_COUNTER_H

#include "box.h"
#include "button.h"
#include "label.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_counter, TGUI_COUNTER, tgui_box)

struct tgui_counter {
	tgui_box_t box;
	tgui_label_t *label;
	tgui_button_t *minus;
	tgui_button_t *plus;
	long value;
	long min;
	long max;
	long incr;
};

tgui_counter_t *tgui_counter_new(void);
void tgui_counter_set_value(tgui_counter_t *counter, long value);
long tgui_counter_get_value(tgui_counter_t *counter);
void tgui_counter_set_range(tgui_counter_t *counter, long min, long max);
void tgui_counter_set_increment(tgui_counter_t *counter, long incr);

#endif
