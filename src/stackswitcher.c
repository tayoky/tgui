#include <stackswitcher.h>
#include <stack.h>
#include <button.h>
#include <box.h>

TOBJECT_DEFINE_CLASS(tgui_stack_switcher, TGUI_STACK_SWITCHER, tgui_box_get_type())

static void tgui_stack_switcher_class_init(tgui_stack_switcher_class_t *class) {
	(void)class;
}

tgui_stack_switcher_t *tgui_stack_switcher_new(void) {
	tgui_stack_switcher_t *stack_switcher = tobject_new(tgui_stack_switcher_get_type());
	if (!stack_switcher) return NULL;

	// use horizontal by default
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(stack_switcher), TGUI_ORIENTATION_HORIZONTAL);

	return stack_switcher;
}

static void tgui_stack_switcher_button_click(tobject_t *tobject) {
	tgui_button_t *button = TGUI_BUTTON_CAST(tobject);
	tgui_stack_switcher_t *stack_switcher = TGUI_STACK_SWITCHER_CAST(TGUI_WIDGET_CAST(tobject)->parent);
	tgui_stack_t *stack = tgui_stack_switcher_get_stack(stack_switcher);
	if (!stack) return; 
	tgui_stack_set_current(stack, tgui_button_get_text(button));
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
		tgui_widget_connect_signal(TGUI_WIDGET_CAST(button), "click", TCALLBACK_CAST(tgui_stack_switcher_button_click), NULL);
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
