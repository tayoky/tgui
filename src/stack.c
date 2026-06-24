#include <stack.h>
#include <widget.h>
#include <stdlib.h>
#include <string.h>

TOBJECT_DEFINE_CLASS(tgui_stack, TGUI_STACK, tgui_widget_get_type())

static void tgui_stack_calculate_sizes(tgui_widget_t *widget) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);
	tgui_widget_t *child = tgui_stack_get_current(stack);
	if (!child) return;
	tgui_widget_calculate_sizes(child);
	widget->min_width  = child->min_width;
	widget->min_height = child->min_height;
	widget->pref_width  = child->pref_width;
	widget->pref_height = child->pref_height;
}

static void tgui_stack_allocate_space(tgui_widget_t *widget) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);
	tgui_widget_t *child = tgui_stack_get_current(stack);
	if (!child) return;
	long x = tgui_widget_get_inner_x(widget);
	long y = tgui_widget_get_inner_y(widget);
	long width  = tgui_widget_get_inner_width(widget);
	long height = tgui_widget_get_inner_height(widget);
	tgui_widget_allocate_space(child, x, y, width, height);
}

static void tgui_stack_remove_child(tgui_widget_t *widget, tgui_widget_t *child) {
	tgui_stack_t *stack = TGUI_STACK_CAST(widget);

	// make sure the page is not focused
	if (tgui_stack_get_current(stack) == child) {
		// TODO : maybee switch to another one
		tgui_stack_set_current_page(stack, NULL);
	}

	tgui_stack_page_t *page = child->layout_data;
	tgui_widget_send_signal(TGUI_WIDGET_CAST(stack), "remove-page", page);
	tgui_list_remove(&stack->pages, &page->node);
	free(page->name);
	free(page);
}

static void tgui_stack_class_init(tgui_stack_class_t *class) {
	tgui_widget_class_t *widget_class = TGUI_WIDGET_CLASS_CAST(class);
	widget_class->calculate_sizes = tgui_stack_calculate_sizes;
	widget_class->allocate_space  = tgui_stack_allocate_space;
	widget_class->remove_child    = tgui_stack_remove_child;
}

tgui_stack_t *tgui_stack_new(void) {
	return tobject_new(tgui_stack_get_type());
}

void tgui_stack_add_child(tgui_stack_t *stack, tgui_widget_t *child, const char *name) {
	tgui_stack_page_t *page = malloc(sizeof(tgui_stack_page_t));
	page->widget = child;
	page->name = strdup(name);
	tgui_list_append(&stack->pages, &page->node);

	tgui_widget_set_parent(child, TGUI_WIDGET_CAST(stack));
	tgui_widget_hide(child);
	child->layout_data = page;
	tgui_widget_send_signal(TGUI_WIDGET_CAST(stack), "add-page", page);
	if (stack->pages.first == &page->node) {
		// we are the first one set as visible
		tgui_stack_set_current_page(stack, page);
	}
}

void tgui_stack_set_current_page(tgui_stack_t *stack, tgui_stack_page_t *page) {
	tgui_widget_send_signal(TGUI_WIDGET_CAST(stack), "switch-page", page);
	if (stack->current) {
		tgui_widget_hide(tgui_stack_get_current(stack));
	}
	stack->current = page;
	if (page) {
		tgui_widget_show(page->widget);
	}
}

tgui_stack_page_t *tgui_stack_get_current_page(tgui_stack_t *stack) {
	return stack->current;
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
	return stack->current ? stack->current->widget : NULL;
}
