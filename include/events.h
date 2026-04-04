#ifndef TGUI_EVENTS_H
#define TGUI_EVENTS_H

struct tgui_widget;

typedef struct tgui_event {
	struct tgui_widget *widget;
	int type;
	void *data;
	union {
		struct {
			int button;
			int x;
			int y;
		} click;
		struct {
			int button;
			int x;
			int y;
		} unclick;
		struct {
			int rel_x;
			int rel_y;
			int abs_x;
			int abs_y;
			int is_pressed;
		} move;
		struct {
			int amount;
		} scroll;
		struct {
			long scancode;
			long sym;
		} press;
		struct {
			long scancode;
			long sym;
		} release;
	};
} tgui_event_t;

#define TGUI_EVENT_CLICK   0
#define TGUI_EVENT_UNCLICK 1
#define TGUI_EVENT_MOVE    2
#define TGUI_EVENT_SCROLL  3
#define TGUI_EVENT_PRESS   4
#define TGUI_EVENT_RELEASE 5
#define TGUI_EVENT_COUNT   6

#define TGUI_EVENT_NOT_HANDLED 0
#define TGUI_EVENT_HANDLED     1

typedef int (*tgui_callback_t)(tgui_event_t *event);
typedef struct tgui_callback_reg {
	tgui_callback_t callback;
	void *data;
} tgui_callback_reg_t;

typedef tgui_callback_reg_t tgui_callbacks_t[TGUI_EVENT_COUNT];

void tgui_event_set_callback(tgui_callbacks_t callbacks, int type, tgui_callback_t callback, void *data);
int tgui_event_report(tgui_callbacks_t callbacks, tgui_event_t *event);

#endif
