#include <stack.h>
#include <widget.h>
#include <stdlib.h>
#include <string.h>

static void tgui_stack_calculate_sizes(tgui_widget_t *widget) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);
	if (!stack->current) return;
	tgui_widget_calculate_sizes(stack->current);
	widget->min_width  = stack->current->min_width;
	widget->min_height = stack->current->min_height;
	widget->pref_width  = stack->current->pref_width;
	widget->pref_height = stack->current->pref_height;
}

static void tgui_stack_allocate_space(tgui_widget_t *widget) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);
	if (!stack->current) return;
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);
	tgui_widget_allocate_space(stack->current, x, y, width, height);
}

static void tgui_stack_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);

	// make sure the page is not focused
	if (stack->current == child) {
		tgui_stack_set_current_page(stack, NULL);
	}

	tgui_stack_page_t *page = child->layout_data;
	tgui_list_remove(&stack->pages, &page->node);
	free(page->name);
	free(page);
}

static tgui_widget_class_t stack_class = {
	.size = sizeof(tgui_stack_t),
	.name = "stack",
	.calculate_sizes = tgui_stack_calculate_sizes,
	.allocate_space  = tgui_stack_allocate_space,
	.remove_child    = tgui_stack_remove_child,
};

tgui_stack_t *tgui_stack_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&stack_class);
	if (!widget) return NULL;

	tgui_stack_t *stack = TGUI_STACK_CAST(widget);
	return stack;
}

void tgui_stack_add_child(tgui_stack_t *stack, tgui_widget_t *child, const char *name) {
	tgui_stack_page_t *page = malloc(sizeof(tgui_stack_page_t));
	page->widget = child;
	page->name = strdup(name);
	tgui_list_append(&stack->pages, &page->node);

	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(stack));
	tgui_widget_hide(child);
	child->layout_data = page;
}

void tgui_stack_set_current_page(tgui_stack_t *stack, tgui_stack_page_t *page) {
	tgui_widget_hide(stack->current);
	if (page) {
		stack->current = page->widget;
		tgui_widget_show(stack->current);
	} else {
		stack->current = NULL;
	}
}

void tgui_stack_set_current(tgui_stack_t *stack, const char *name) {
	tgui_stack_page_t *page = tgui_stack_get_page(stack, name);
	if (!page) return;
	tgui_stack_set_current_page(stack, page);
}

tgui_list_t *tgui_stack_get_pages(tgui_stack_t *stack) {
	return &stack->pages;
}
tgui_stack_page_t *tgui_stack_get_page(tgui_stack_t *stack, const char *name) {
	TGUI_LIST_FOREACH(node, &stack->pages) {
		tgui_stack_page_t *page = TGUI_STACK_PAGE_CAST(node);
		if (!strcmp(page->name, name)) {
			return page;
		}
	}
	return NULL;
}

tgui_widget_t *tgui_stack_get_child(tgui_stack_t *stack, const char *name) {
	tgui_stack_page_t *page = tgui_stack_get_page(stack, name);
	return page ? page->widget : NULL;
}

tgui_widget_t *tgui_stack_get_current(tgui_stack_t *stack) {
	return stack->current;
}
