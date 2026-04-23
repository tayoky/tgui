#ifndef TGUI_STACK_SWITCHER_H
#define TGUI_STACK_SWITCHER_H

#include "box.h"
#include "stack.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_stack_switcher, TGUI_STACK_SWITCHER, tgui_box)

struct tgui_stack_switcher {
	tgui_box_t box;
	tgui_stack_t *stack;
	size_t add_page_callback;
	size_t remove_page_callback;
	size_t destroy_callback;
};

tgui_stack_switcher_t *tgui_stack_switcher_new(void);
void tgui_stack_switcher_set_stack(tgui_stack_switcher_t *stack_switcher, tgui_stack_t *stack);
tgui_stack_t *tgui_stack_switcher_get_stack(tgui_stack_switcher_t *stack_switcher);

#endif
