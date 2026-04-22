#include <tobject.h>
#include <stdlib.h>

TOBJECT_DEFINE_CLASS(tobject, TOBJECT, NULL)

// define a few std classes
TOBJECT_DEFINE_CLASS(tobject_int, TOBJECT_INT, tobject_get_type())
TOBJECT_DEFINE_CLASS(tobject_uint, TOBJECT_UINT, tobject_get_type())
TOBJECT_DEFINE_CLASS(tobject_string, TOBJECT_STRING, tobject_get_type())

static void tobject_int_class_init(tobject_int_class_t *class) {
	(void)class;
}

static void tobject_uint_class_init(tobject_uint_class_t *class) {
	(void)class;
}

static void tobject_string_class_init(tobject_string_class_t *class) {
	(void)class;
}

static int do_nothing(void *object) {
	(void)object;
	return 0;
}

static void tobject_class_init(tobject_class_t *class) {
	class->constructor = do_nothing;
	class->destructor  = do_nothing;
}

void *tobject_new(ttype_t *type) {
	tobject_t *object = malloc(type->size);
	if (!object) return NULL;
	memset(object, 0, type->size);
	object->type = type;

	// call constructor
	if (type->class->constructor) {
		type->class->constructor(object);
	}

	return object;
}

void tobject_free(void *object) {
	ttype_t *type = tobject_type_from_object(object);

	// call destructor
	if (type->class->destructor) {
		type->class->destructor(object);
	}
	free(object);
}

static thandler_group_t *get_handler_group(tobject_t *tobject, const char *signal) {
	thandler_group_t *group = tobject->handler_groups;
	while (group) {
		if (!strcmp(group->signal, signal)) {
			return group;
		}
		group = group->next;
	}
	return NULL;
}

void tobject_send_signal(tobject_t *tobject, const char *signal, void *event) {
	thandler_group_t *group = get_handler_group(tobject, signal);
	if (!group) return;

	thandler_t *handler = group->handlers;
	while (handler) {
		handler->callback(tobject, event, handler->user_data);
		handler = handler->next;
	}
}
size_t tobject_connect_signal(tobject_t *tobject, const char *signal, tcallback_t callback, void *user_data) {
	// TODO
}
void tobject_disconnect_signal(tobject_t *tobject, size_t id) {
	// TODO
}

void tobject_set_property(tobject_t *tobject, const char *name, const void *value) {
	ttype_t *type = tobject_type_from_object(tobject);
	for (;type; type = type->parent_type) {
		if (!type->properties) {
			continue;
		}
		for (tproperty_t *prop = type->properties; prop->name; prop++) {
			if (strcmp(prop->name, name)) {
				continue;
			}
			type->class->set_property(tobject, prop->id, value);
			break;
		}
	}
}

void tobject_get_property(tobject_t *tobject, const char *name, void *value) {
	ttype_t *type = tobject_type_from_object(tobject);
	for (;type; type = type->parent_type) {
		if (!type->properties) {
			continue;
		}
		for (tproperty_t *prop = type->properties; prop->name; prop++) {
			if (strcmp(prop->name, name)) {
				continue;
			}
			type->class->get_property(tobject, prop->id, value);
			break;
		}
	}
}
