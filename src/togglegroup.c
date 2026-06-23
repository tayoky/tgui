#include <togglegroup.h>
#include <stdlib.h>

TOBJECT_DEFINE_CLASS(tgui_toggle_group, TGUI_TOGGLE_GROUP, tobject_get_type())

static void tgui_toggle_group_toggled(tgui_toggle_button_t *toggled_button, int *active, tgui_toggle_group_element_t *toggled_element) {
	tgui_toggle_group_t *group = toggled_element->group;
	if (*active == 0) {
		// we desactivated a button, we don't care
		return;
	}

	// we need to set every single other button as inactivated
	TGUI_LIST_FOREACH(node, &group->elements) {
		tgui_toggle_group_element_t *element = TGUI_CONTAINER_OF(node, tgui_toggle_group_element_t, node);
		if (element->button != toggled_button) {
			tgui_toggle_button_set_active(element->button, 0);
		}
	}
}

static void tgui_toggle_group_destroy(tgui_toggle_button_t *toggle_button, int stub, tgui_toggle_group_element_t *element) {
	(void)toggle_button;
	(void)stub;
	tgui_toggle_group_t *group = element->group;
	tgui_toggle_group_remove(group, element);
}

static int tgui_toggle_group_destructor(void *object) {
	tgui_toggle_group_t *group = TGUI_TOGGLE_GROUP_CAST(object);
	for (tgui_list_node_t *node = group->elements.first; node;) {
		tgui_list_node_t *next = node->next;
		tgui_toggle_group_element_t *element = TGUI_CONTAINER_OF(node, tgui_toggle_group_element_t, node);
		tgui_toggle_group_remove(group, element);
		node = next;
	}
	return tgui_toggle_group_get_parent_class()->destructor(object);
}

static inline void tgui_toggle_group_class_init(tgui_toggle_group_class_t *class) {
	tobject_class_t *tobject_class = TOBJECT_CLASS_CAST(class);
	tobject_class->destructor = tgui_toggle_group_destructor;
}

tgui_toggle_group_t *tgui_toggle_group_new(void) {
	return tobject_new(tgui_toggle_group_get_type());
}

tgui_toggle_group_element_t *tgui_toggle_group_add(tgui_toggle_group_t *group, tgui_toggle_button_t *button) {
	tgui_toggle_group_element_t *element = malloc(sizeof(tgui_toggle_group_element_t));
	element->group  = group;
	element->button = button;
	element->toggled = tgui_widget_connect_signal(TGUI_WIDGET_CAST(button), "toggled", TCALLBACK_CAST(tgui_toggle_group_toggled), element);
	element->destroy = tgui_widget_connect_signal(TGUI_WIDGET_CAST(button), "destroy", TCALLBACK_CAST(tgui_toggle_group_destroy), element);
	tgui_list_append(&group->elements, &element->node);
	return element;
}

void tgui_toggle_group_remove(tgui_toggle_group_t *group, tgui_toggle_group_element_t *element) {
	tgui_widget_disconnect_signal(TGUI_WIDGET_CAST(element->button), "toggled", element->toggled);
	tgui_widget_disconnect_signal(TGUI_WIDGET_CAST(element->button), "destroy", element->destroy);
	tgui_list_remove(&group->elements, &element->node);
	free(element);
}

void tgui_toggle_group_reset(tgui_toggle_group_t *group) {
	TGUI_LIST_FOREACH(node, &group->elements) {
		tgui_toggle_group_element_t *element = TGUI_CONTAINER_OF(node, tgui_toggle_group_element_t, node);
		tgui_toggle_button_set_active(element->button, 0);
	}
}
