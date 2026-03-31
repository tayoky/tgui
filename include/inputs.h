#ifndef TGUI_INPUT_H
#define TGUI_INPUT_H

#include "window.h"

#define TGUI_BUTTON_LEFT   0
#define TGUI_BUTTON_MIDDLE 1
#define TGUI_BUTTON_RIGHT  2

void tgui_input_click(tgui_window_t *window, int button, long x, long y);
void tgui_input_unclick(tgui_window_t *window, int button, long x, long y);
void tgui_input_move(tgui_window_t *window, long x, long y);
void tgui_input_key_press(tgui_window_t *window, long scancode, long sym);
void tgui_input_key_release(tgui_window_t *window, long scancode, long sym);

#endif
