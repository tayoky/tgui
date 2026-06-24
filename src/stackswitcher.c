#include <stackswitcher.h>
#include <stack.h>
#include <button.h>
#include <togglebutton.h>
#include <togglegroup.h>
#include <box.h>

TOBJECT_DEFINE_CLASS(tgui_stack_switcher, TGUI_STACK_SWITCHER, tgui_box_get_type())

static int tgui_stack_switcher_constructor(void *object) {
	tgui_stack_switcher_get_parent_class()->constructor(object);

	tgui_stack_switcher_t *stack_switcher = TGUI_STACK_SWITCHER_CAST(object);
	stack_switcher->toggle_group = tgui_toggle_group_new();
	return 0;
}

static int tgui_stack_switcher_destructor(void *object) {
	// on destruction disconnect from any stack
	tgui_stack_switcher_t *stack_switcher = TGUI_STACK_SWITCHER_CAST(object);
	tgui_stack_switcher_set_stack(stack_switcher, NULL);
	tgui_toggle_group_free(stack_switcher->toggle_group);
	
	return tgui_stack_switcher_get_parent_class()->destructor(object);
}

static void tgui_stack_switcher_class_init(tgui_stack_switcher_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->constructor = tgui_stack_switcher_constructor;
	tobject_class->destructor  = tgui_stack_switcher_destructor;
}

tgui_stack_switcher_t *tgui_stack_switcher_new(void) {
	tgui_stack_switcher_t *stack_switcher = tobject_new(tgui_stack_switcher_get_type());
	if (!stack_switcher) return NULL;

	// use horizontal by default
	tgui_widget_set_orientation(TGUI_WIDGET_CAST(stack_switcher), TGUI_ORIENTATION_HORIZONTAL);

	return stack_switcher;
}

static void tgui_stack_switcher_button_toggled(tobject_t *tobject, int *active) {
	tgui_button_t *button = TGUI_BUTTON_CAST(tobject);
	tgui_stack_switcher_t *stack_switcher = TGUI_STACK_SWITCHER_CAST(TGUI_WIDGET_CAST(button)->parent);
	tgui_stack_t *stack = tgui_stack_switcher_get_stack(stack_switcher);
	if (!stack) return; 
	if (!*active) return;
	tgui_stack_set_current(stack, tgui_button_get_text(button));
}

static void tgui_stack_switcher_add_page_button(tgui_stack_switcher_t *stack_switcher, tgui_stack_page_t *page) {
	tgui_toggle_button_t *button = tgui_toggle_button_new();
	tgui_button_set_text(TGUI_BUTTON_CAST(button), page->name);
	tgui_widget_connect_signal(TGUI_WIDGET_CAST(button), "toggled", TCALLBACK_CAST(tgui_stack_switcher_button_toggled), NULL);
	tgui_widget_apply_class_styles(TGUI_WIDGET_CAST(button), "stackswitcher-button");
	tgui_widget_set_hexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
	tgui_widget_set_vexpand(TGUI_WIDGET_CAST(button), TGUI_TRUE);
	tgui_toggle_group_add(stack_switcher->toggle_group, button);
	tgui_box_append_widget(TGUI_BOX_CAST(stack_switcher), TGUI_WIDGET_CAST(button));
}

static void tgui_stack_switcher_add_page(tobject_t *tobject, tgui_stack_page_t *page, tgui_stack_switcher_t *stack_switcher) {
	(void)tobject;
	tgui_stack_switcher_add_page_button(stack_switcher, page);
}

static void tgui_stack_switcher_remove_page(tobject_t *tobject, tgui_stack_page_t *page, tgui_stack_switcher_t *stack_switcher) {
	(void)tobject;
	TGUI_LIST_FOREACH(node, &TGUI_WIDGET_CAST(stack_switcher)->children) {
		tgui_widget_t *widget = TGUI_WIDGET_FROM_NODE(node);
		tgui_button_t *button = TGUI_BUTTON_CAST(widget);
		if (!strcmp(tgui_button_get_text(button), page->name)) {
			tgui_widget_destroy(widget);
		}
	}	
}

static void tgui_stack_switcher_destroy_stack(tobject_t *tobject, void *event, tgui_stack_switcher_t *stack_switcher) {
	(void)tobject;
	(void)event;
	
	// if the stack is destroyed 
	// we must deconnect the stack switcher from it
	tgui_stack_switcher_set_stack(stack_switcher, NULL);
}

void tgui_stack_switcher_set_stack(tgui_stack_switcher_t *stack_switcher, tgui_stack_t *stack) {
	// unsuscribe from the last stack
	if (stack_switcher->stack) {
		tgui_widget_disconnect_signal(TGUI_WIDGET_CAST(stack_switcher->stack), "add-page", stack_switcher->add_page_callback);
		tgui_widget_disconnect_signal(TGUI_WIDGET_CAST(stack_switcher->stack), "remove-page", stack_switcher->remove_page_callback);
		tgui_widget_disconnect_signal(TGUI_WIDGET_CAST(stack_switcher->stack), "destroy", stack_switcher->destroy_callback);

		// make sure to destroy any previous buttons
		while (TGUI_WIDGET_CAST(stack_switcher)->children.first) {
			tgui_widget_t *child = TGUI_WIDGET_FROM_NODE(TGUI_WIDGET_CAST(stack_switcher)->children.first);
			tgui_widget_destroy(child);
		}
	}

	stack_switcher->stack = stack;
	if (!stack) return;

	TGUI_LIST_FOREACH(node, tgui_stack_get_pages(stack_switcher->stack)) {
		tgui_stack_page_t *page = TGUI_STACK_PAGE_CAST(node);
		tgui_stack_switcher_add_page_button(stack_switcher, page);
	}

	// register so we can add buttons for future pages
	stack_switcher->add_page_callback = tgui_widget_connect_signal(TGUI_WIDGET_CAST(stack), "add-page", TCALLBACK_CAST(tgui_stack_switcher_add_page), stack_switcher);
	stack_switcher->remove_page_callback = tgui_widget_connect_signal(TGUI_WIDGET_CAST(stack), "remove-page", TCALLBACK_CAST(tgui_stack_switcher_remove_page), stack_switcher);
	stack_switcher->destroy_callback = tgui_widget_connect_signal(TGUI_WIDGET_CAST(stack), "destroy", TCALLBACK_CAST(tgui_stack_switcher_destroy_stack), stack_switcher);
}

tgui_stack_t *tgui_stack_switcher_get_stack(tgui_stack_switcher_t *stack_switcher) {
	return stack_switcher->stack;
}
