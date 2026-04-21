#ifndef TGUI_H
#define TGUI_H

#include "widget.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "list.h"
#include "stringlist.h"
#include "factory.h"
#include "stringfactory.h"
#include "box.h"
#include "grid.h"
#include "text.h"
#include "paned.h"
#include "slider.h"
#include "scrollbar.h"
#include "titlebar.h"
#include "stack.h"
#include "stackswitcher.h"
#include "icon.h"
#include "canva.h"
#include "expander.h"
#include "separator.h"
#include "listview.h"
#include "popover.h"
#include "popoverbutton.h"
#include "dropdown.h"
#include "menuitem.h"
#include "submenubutton.h"
#include "font.h"
#include "color.h"
#include "inputs.h"

void tgui_quit(void);
int tgui_init(void);
void tgui_fini(void);
void tgui_main(void);
int tgui_get_fd(void);
void tgui_render(void);
void tgui_poll(void);

#endif
