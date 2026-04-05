#ifndef TGUI_STACK_SWITCHER_H
#define TGUI_STACK_SWITCHER_H

#include "box.h"
#include "stack.h"

typedef struct tgui_stack_switcher {
	tgui_box_t box;
	tgui_stack_t *stack;
} tgui_stack_switcher_t;

#define TGUI_STACK_SWITCHER_CAST(w) TGUI_CONTAINER_OF(w, tgui_stack_switcher_t, box.widget)

tgui_stack_switcher_t *tgui_stack_switcher_new(void);
void tgui_stack_switcher_update(tgui_stack_switcher_t *stack_switcher);
void tgui_stack_switcher_set_stack(tgui_stack_switcher_t *stack_switcher, tgui_stack_t *stack);
tgui_stack_t *tgui_stack_switcher_get_stack(tgui_stack_switcher_t *stack_switcher);

#endif
