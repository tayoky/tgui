#include <stackswitcher.h>
#include <stack.h>
#include <button.h>
#include <box.h>

static tgui_widget_class_t stack_switcher_class = {
	.size = sizeof(tgui_stack_switcher_t),
	.name = "stack switcher",

	// mostly a box
	.calculate_sizes = tgui_box_calculate_sizes,
	.allocate_space  = tgui_box_allocate_space,
};

tgui_stack_switcher_t *tgui_stack_switcher_new(void) {
	tgui_widget_t *widget = tgui_widget_new(&stack_switcher_class);
	if (!widget) return NULL;

	// use horizontal by default
	tgui_widget_set_orientation(widget, TGUI_ORIENTATION_HORIZONTAL);

	return TGUI_STACK_SWITCHER_CAST(widget);
}

static int tgui_stack_switcher_button_click(tgui_event_t *event) {
	tgui_button_t *button = TGUI_BUTTON_CAST(event->widget);
	tgui_stack_switcher_t *stack_switcher = TGUI_STACK_SWITCHER_CAST(event->widget->parent);
	tgui_stack_t *stack = tgui_stack_switcher_get_stack(stack_switcher);
	if (!stack) return TGUI_EVENT_HANDLED;
	tgui_stack_set_current(stack, tgui_button_get_text(button));
	return TGUI_EVENT_HANDLED;
}
	

void tgui_stack_switcher_update(tgui_stack_switcher_t *stack_switcher) {
	// make sure to destroy any previous buttons
	while (stack_switcher->box.widget.children.first) {
		tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(stack_switcher->box.widget.children.first);
		tgui_widget_destroy(child);
	}

	if (!stack_switcher->stack) return;
	TGUI_LIST_FOREACH(node, tgui_stack_get_pages(stack_switcher->stack)) {
		tgui_stack_page_t *page = TGUI_STACK_PAGE_CAST(node);
		tgui_button_t *button = tgui_button_new();
		tgui_button_set_text(button, page->name);
		tgui_widget_set_callback(TGUI_WIDGET_CAST(button), TGUI_EVENT_CLICK, tgui_stack_switcher_button_click, NULL);
		tgui_widget_set_hexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
		tgui_widget_set_vexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
		tgui_box_append_widget(&stack_switcher->box, TGUI_WIDGET_CAST(button));
	}
}

void tgui_stack_switcher_set_stack(tgui_stack_switcher_t *stack_switcher, tgui_stack_t *stack) {
	stack_switcher->stack = stack;
	tgui_stack_switcher_update(stack_switcher);
}

tgui_stack_t *tgui_stack_switcher_get_stack(tgui_stack_switcher_t *stack_switcher) {
	return stack_switcher->stack;
}
