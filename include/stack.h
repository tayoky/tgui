#ifndef TGUI_STACK_H
#define TGUI_STACK_H

#include "widget.h"

typedef struct tgui_stack_page {
	tgui_list_node_t node;
	tgui_widget_t *widget;
	char *name;
} tgui_stack_page_t;

#define TGUI_STACK_PAGE_CAST(n) TGUI_CONTAINER_OF(n, tgui_stack_page_t, node)

typedef struct tgui_stack {
	tgui_widget_t widget;
	tgui_list_t pages;
	tgui_widget_t *current;
} tgui_stack_t;

#define TGUI_STACK_CAST(w) TGUI_CONTAINER_OF(w, tgui_stack_t, widget)

tgui_stack_t *tgui_stack_new(void);
void tgui_stack_add_child(tgui_stack_t *stack, tgui_widget_t *child, const char *name);
void tgui_stack_set_current(tgui_stack_t *stack, const char *name);
void tgui_stack_set_current_page(tgui_stack_t *stack, tgui_stack_page_t *page);
tgui_list_t *tgui_stack_get_pages(tgui_stack_t *stack);
tgui_stack_page_t *tgui_stack_get_page(tgui_stack_t *stack, const char *name);
tgui_widget_t *tgui_stack_get_child(tgui_stack_t *stack, const char *name);
tgui_widget_t *tgui_stack_get_current(tgui_stack_t *stack);

#endif
