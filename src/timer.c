#include <timer.h>
#include <time.h>

TOBJECT_DEFINE_CLASS(tgui_timer, TGUI_TIMER, tobject_get_type())

static tgui_list_t timers;

static int tgui_timer_constructor(void *object) {
	tgui_timer_get_parent_class()->constructor(object);
	
	tgui_timer_t *timer = TGUI_TIMER_CAST(object);
	timer->duration = 0;
	timer->trigger  = -1;
	return 0;
}

static int tgui_timer_destructor(void *object) {
	tgui_timer_t *timer = TGUI_TIMER_CAST(object);
	if (timer->trigger != -1) {
		tgui_list_remove(&timers, &timer->node);
	}
	return tgui_timer_get_parent_class()->destructor(object);
}

static void tgui_timer_class_init(tgui_timer_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_timer_constructor;
	tobject_class->destructor  = tgui_timer_destructor;
}

tgui_timer_t *tgui_timer_new(long duration) {
	tgui_timer_t *timer = tobject_new(tgui_timer_get_type());
	if (!timer) return NULL;
	timer->duration = duration;
	tgui_timer_reset(timer);
	return tgui_timer_ref(timer);
}

void tgui_timer_reset(tgui_timer_t *timer) {
	if (timer->trigger != -1) {
		// it was already in the list remove it
		tgui_list_remove(&timers, &timer->node);
	}
	timer->trigger = tgui_timer_get_current_time() + timer->duration;

	// we need to keep the list sorted
	tgui_list_node_t *prev = NULL;
	TGUI_LIST_FOREACH (node, &timers) {
		tgui_timer_t *current = TGUI_CONTAINER_OF(node, tgui_timer_t, node);
		if (current->trigger > timer->trigger) {
			break;
		}
		prev = node;
	}
	tgui_list_add_after(&timers, prev, &timer->node);
}

tgui_time_t tgui_timer_get_current_time(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

tgui_time_t tgui_timer_get_next_trigger(void) {
	if (tgui_list_is_empty(&timers)) {
		return -1;
	} else {
		tgui_timer_t *timer = TGUI_CONTAINER_OF(timers.first, tgui_timer_t, node);
		return timer->trigger;
	}
}

tgui_time_t tgui_timer_get_before_next_trigger(void) {
	tgui_time_t next = tgui_timer_get_next_trigger();
	if (next == -1) return -1;
	tgui_time_t current = tgui_timer_get_current_time();
	if (current >= next) {
		return 0;
	} else {
		return next - current;
	}
}

void tgui_timer_update(void) {
	tgui_time_t current = tgui_timer_get_current_time();
	while (!tgui_list_is_empty(&timers)) {
		tgui_timer_t *timer = TGUI_CONTAINER_OF(timers.first, tgui_timer_t, node);
		if (timer->trigger > current) break;
		timer->trigger = -1;
		tgui_list_remove(&timers, &timer->node);
		tgui_timer_send_signal(timer, "trigger", NULL);
	}
}
