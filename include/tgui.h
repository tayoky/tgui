#ifndef TGUI_H
#define TGUI_H

#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "list.h"
#include "box.h"
#include "expander.h"
#include "font.h"
#include "color.h"

void tgui_quit(void);
int tgui_init(void);
void tgui_fini(void);
void tgui_main(void);

#endif
