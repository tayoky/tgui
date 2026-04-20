#ifndef TGUI_INPUT_H
#define TGUI_INPUT_H

#include "surface.h"

#define TGUI_BUTTON_LEFT   0
#define TGUI_BUTTON_MIDDLE 1
#define TGUI_BUTTON_RIGHT  2

#define TGUI_KEY_BACKSPACE    '\177'
#define TGUI_KEY_TAB          '\t'
#define TGUI_KEY_ESC          '\033'
#define TGUI_KEY_ENTER        '\r'
#define TGUI_KEY_FIRST        17000000
#define TGUI_KEY_MOUSE_LEFT   17000000
#define TGUI_KEY_MOUSE_MIDDLE 17000001
#define TGUI_KEY_MOUSE_RIGHT  17000002
#define TGUI_KEY_DELETE       17000055
#define TGUI_KEY_INSERT       17000056
#define TGUI_KEY_HOME         17000057
#define TGUI_KEY_END          17000058
#define TGUI_KEY_ARROW_UP     17000060
#define TGUI_KEY_ARROW_DOWN   17000061
#define TGUI_KEY_ARROW_LEFT   17000062
#define TGUI_KEY_ARROW_RIGHT  17000063
#define TGUI_KEY_PAGE_UP      17000064
#define TGUI_KEY_PAGE_DOWN    17000065
#define TGUI_KEY_LSHIFT       17000066
#define TGUI_KEY_RSHIFT       17000067
#define TGUI_KEY_LCRTL        17000068
#define TGUI_KEY_RCRTL        17000069
#define TGUI_KEY_LALT         17000070
#define TGUI_KEY_RALT         17000071
#define TGUI_KEY_ALTGR        17000072
#define TGUI_KEY_NUM_LOCK     17000073
#define TGUI_KEY_SCROLL_LOCK  17000074
#define TGUI_KEY_CAPS_LOCK    17000075
#define TGUI_KEY_F1           17000076
#define TGUI_KEY_F2           17000077
#define TGUI_KEY_F3           17000078
#define TGUI_KEY_F4           17000079
#define TGUI_KEY_F5           17000080
#define TGUI_KEY_F6           17000081
#define TGUI_KEY_F7           17000082
#define TGUI_KEY_F8           17000083
#define TGUI_KEY_F9           17000084
#define TGUI_KEY_F10          17000085
#define TGUI_KEY_F11          17000086
#define TGUI_KEY_F12          17000087
#define TGUI_KEY_GUI          17000088
#define TGUI_KEY_VOLUME_UP    17000089
#define TGUI_KEY_VOLUME_DOWN  17000090
#define TGUI_KEY_LAST         17000090

void tgui_input_click(tgui_surface_t *surface, int button, long x, long y);
void tgui_input_unclick(tgui_surface_t *surface, int button, long x, long y);
void tgui_input_move(tgui_surface_t *surface, long x, long y);
void tgui_input_focus(tgui_surface_t *surface);
void tgui_input_unfocus(tgui_surface_t *surface);
void tgui_input_key_press(tgui_surface_t *surface, long scancode, long sym);
void tgui_input_key_release(tgui_surface_t *surface, long scancode, long sym);

#endif
