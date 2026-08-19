#ifndef TGUI_TIMER_H
#define TGUI_TIMER_H

#include "list.h"
#include "tobject.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_timer, TGUI_TIMER, tobject)

typedef long tgui_time_t; // time in ms

struct tgui_timer {
	tobject_t tobject;
	tgui_list_node_t node;
	tgui_time_t duration;
	tgui_time_t trigger;
};

tgui_timer_t *tgui_timer_new(tgui_time_t duration);
void tgui_timer_reset(tgui_timer_t *timer);
static inline void tgui_timer_release(tgui_timer_t *timer) {
	tobject_free(TOBJECT_CAST(timer));
}
tgui_time_t tgui_timer_get_current_time(void);
tgui_time_t tgui_timer_get_next_trigger(void);

/**
 * @brief return time before next trigger
 * @return time in ms before next trigger or -1 if no timer are pending
 */
tgui_time_t tgui_timer_get_before_next_trigger(void);

void tgui_timer_update(void);

#endif
