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
	object->ref_count = 0;

	// call constructor
	if (type->class->constructor) {
		type->class->constructor(object);
	}

	return object;
}

static void free_handler_group(thandler_group_t *group) {
	thandler_t *handler = group->handlers;
	while (handler) {
		thandler_t *next = handler->next;
		free(handler);
		handler = next;
	};
	free(group->signal);
	free(group);
}

void tobject_free(tobject_t *tobject) {
	if (!tobject) return;
	if (tobject->ref_count > 1) {
		tobject->ref_count--;
		return;
	}
	ttype_t *type = tobject_type_from_object(tobject);

	// call destructor
	if (type->class->destructor) {
		type->class->destructor(tobject);
	}

	// free signal infrastructure
	thandler_group_t *group = tobject->handler_groups;
	while (group) {
		thandler_group_t *next = group->next;
		free_handler_group(group);
		group = next;
	}
	free(tobject);
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

void __tobject_send_signal(tobject_t *tobject, const char *signal, void *event) {
	if (!tobject) return;
	thandler_group_t *group = get_handler_group(tobject, signal);
	if (!group) return;

	tobject_ref(tobject);
	thandler_t *handler = group->handlers;
	while (handler) {
		thandler_t *next = handler->next;
		handler->callback(tobject, event, handler->user_data);
		handler = next;
	}
	tobject_free(tobject);
}

size_t __tobject_connect_signal(tobject_t *tobject, const char *signal, tcallback_t callback, void *user_data) {
	thandler_group_t *group = get_handler_group(tobject, signal);
	if (!group) {
		group = malloc(sizeof(thandler_group_t));
		memset(group, 0, sizeof(thandler_group_t));
		group->next = tobject->handler_groups;
		tobject->handler_groups = group;
		group->signal = strdup(signal);
	}

	thandler_t *handler = malloc(sizeof(thandler_t));
	static size_t handlers_id = 1;
	handler->id = handlers_id++;
	handler->callback = callback;
	handler->user_data = user_data;
	handler->next = group->handlers;
	group->handlers = handler;
	return handler->id;
}

void __tobject_disconnect_signal(tobject_t *tobject, const char *signal, size_t id) {
	thandler_group_t *group = get_handler_group(tobject, signal);
	if (!group) return;
	thandler_t *handler = group->handlers;
	thandler_t *prev = NULL;
	while (handler) {
		if (handler->id == id) {
			if (prev) {
				prev->next = handler->next;
			} else {
				group->handlers = handler->next;
			}
			free(handler);
			break;
		}
		prev = handler;
		handler = handler->next;
	}
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
