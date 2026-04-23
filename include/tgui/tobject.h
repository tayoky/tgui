#ifndef TOBJECT_H
#define TOBJECT_H

#include <stddef.h>
#include <string.h>

#ifdef __GNUC__
#define TOBJECT_ATTRIBUTE_USED __attribute__((used))
#else
#define TOBJECT_ATTRIBUTE_USED
#endif

#define TOBJECT_DECLARE_CLASS(class_name, CLASS) \
	typedef struct class_name class_name ## _t;\
	typedef struct class_name ## _class class_name ## _class_t;\
	static inline class_name ## _t *CLASS ## _CAST(void *ptr) {\
		return (class_name ## _t*)ptr;\
	}\
	static inline class_name ## _class_t *CLASS ## _CLASS_CAST(void *ptr) {\
		return (class_name ## _class_t*)ptr;\
	}\
	static inline ttype_t *class_name ## _type_from_object(class_name ## _t *object) {\
		if (!object) return NULL;\
		return TOBJECT_CAST(object)->type;\
	}\
	ttype_t *class_name ## _get_type(void);\
	ttype_t *class_name ## _get_type(void);\
	static inline class_name ## _class_t *class_name ## _get_class(class_name ## _t *object) {\
		return CLASS ## _CLASS_CAST(class_name ## _type_from_object(object)->class);\
	}\
	static inline int class_name ## _is_type(class_name ## _t *object, ttype_t *type) {\
		ttype_t *obj_type = class_name ##_type_from_object(object);\
		while (obj_type) {\
			if (obj_type == type) {\
				return 1;\
			}\
			obj_type = obj_type->parent_type;\
		}\
		return 0;\
	}\
	static inline int class_name ## _is_final_type(class_name ## _t *object, ttype_t *type) {\
		return class_name ## _type_from_object(object) == type;\
	}\
	static inline void class_name ## _send_signal(class_name ## _t *object, const char *signal, void *event) {\
		__tobject_send_signal(TOBJECT_CAST(object), signal, event);\
	}\
	static inline size_t class_name ## _connect_signal(class_name ## _t *object, const char *signal, tcallback_t callback, void *user_data) {\
		return __tobject_connect_signal(TOBJECT_CAST(object), signal, callback, user_data);\
	}\
	static inline void class_name ## _disconnect_signal(class_name ## _t *object, const char *signal, size_t id) {\
		__tobject_disconnect_signal(TOBJECT_CAST(object), signal, id);\
	}

#define TOBJECT_DECLARE_SIMPLE_CLASS(class, CLASS, parent) \
	TOBJECT_DECLARE_CLASS(class, CLASS) \
	struct class ## _class {\
		parent ## _class_t parent_class;\
	};


#define TOBJECT_DEFINE_CLASS(class_name, CLASS, parent) \
	static void class_name ## _class_init(class_name ## _class_t *class);\
	static class_name ## _class_t class_name ## _class = {\
	};\
	static ttype_t class_name ## _type = {\
		.size = sizeof(class_name ## _t),\
		.class_size = sizeof(class_name ## _class_t),\
		.name = # class_name,\
	};\
	ttype_t *class_name ## _get_type(void) {\
		if (!class_name ## _type.is_init) {\
			class_name ## _type.parent_type = parent;\
			class_name ## _type.class = TOBJECT_CLASS_CAST(&class_name ## _class);\
			if (class_name ## _type.parent_type) {\
				memcpy(&class_name ## _class, class_name ## _type.parent_type->class, class_name ## _type.parent_type->class_size);\
			}\
			class_name ## _class_init(&class_name ## _class);\
			class_name ## _type.is_init = 1;\
		}\
		return &class_name ## _type;\
	}\
	static TOBJECT_ATTRIBUTE_USED tobject_class_t *class_name ## _get_parent_class(void) {\
		return class_name ## _get_type()->parent_type->class;\
	}

struct tobject_class;
struct tobject;
typedef struct ttype ttype_t;
typedef struct tproperty tproperty_t;
typedef struct thandler thandler_t;
typedef struct thandler_group thandler_group_t;
typedef void (*tcallback_t)(struct tobject *tobject, void *event, void *user_data);

#define TCALLBACK_CAST(ptr) ((tcallback_t)(void*)ptr)

struct ttype {
	int is_init;
	size_t size;
	size_t class_size;
	const char *name;
	ttype_t *parent_type;
	struct tobject_class *class;
	tproperty_t *properties;
};

struct tproperty {
	char *name;
	ttype_t *type;
	unsigned int id;
};

#define TPROPERTY(_name, _type) {.name = _name, .type = _type}

struct thandler {
	thandler_t *next;
	tcallback_t callback;
	void *user_data;
	size_t id;
};

struct thandler_group {
	thandler_group_t *next;
	thandler_t *handlers;
	char *signal;
};

struct tobject {
	ttype_t *type;
	thandler_group_t *handler_groups;
	size_t ref_count;
};

void __tobject_send_signal(struct tobject *tobject, const char *signal, void *event);
size_t __tobject_connect_signal(struct tobject *tobject, const char *signal, tcallback_t callback, void *user_data);
void __tobject_disconnect_signal(struct tobject *tobject, const char *signal, size_t id);
TOBJECT_DECLARE_CLASS(tobject, TOBJECT)

struct tobject_class {
	int (*constructor)(void *object);
	int (*destructor)(void *object);
	void (*set_property)(tobject_t *tobject, unsigned int property_id, const void *value);
	void (*get_property)(tobject_t *tobject, unsigned int property_id, void *value);
};


// declare a few std classes
TOBJECT_DECLARE_SIMPLE_CLASS(tobject_int, TOBJECT_INT, tobject)
TOBJECT_DECLARE_SIMPLE_CLASS(tobject_uint, TOBJECT_UINT, tobject)
TOBJECT_DECLARE_SIMPLE_CLASS(tobject_string, TOBJECT_STRING, tobject)

struct tobject_int {
	long integer;
};

struct tobject_uint {
	unsigned long uinteger;
};

struct tobject_string {
	char *str;
};

static inline tobject_class_t *ttype_get_class(ttype_t *type) {
	return type->class;
}

static inline ttype_t *ttype_get_parent(ttype_t *type) {
	return type->parent_type;
}

void *tobject_new(ttype_t *type);
void tobject_free(tobject_t *tobject);
static inline tobject_t *tobject_ref(tobject_t *tobject) {
	if (tobject) tobject->ref_count++;
	return tobject;
}
void tobject_set_property(tobject_t *tobject, const char *name, const void *value);
void tobject_get_property(tobject_t *tobject, const char *name, void *value);

static inline void tobject_install_properties(ttype_t *type, tproperty_t *properties) {
	type->properties = properties;
}

#endif
