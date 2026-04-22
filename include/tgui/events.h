#ifndef TGUI_EVENTS_H
#define TGUI_EVENTS_H

struct tgui_widget;

typedef struct tgui_event_click {
	int button;
	int x;
	int y;
} tgui_event_click_t;

typedef struct tgui_event_unclick {
	int button;
	int x;
	int y;
} tgui_event_unclick_t;

typedef struct tgui_event_move {
	int rel_x;
	int rel_y;
	int abs_x;
	int abs_y;
	int is_pressed;
} tgui_event_move_t;

typedef struct tgui_event_scroll {
	int amount;
} tgui_event_scroll_t;

typedef struct tgui_event_press {
	long scancode;
	long sym;
} tgui_event_press_t;

typedef struct tgui_event_release {
	long scancode;
	long sym;
} tgui_event_release_t;

#endif
