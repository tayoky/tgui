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

typedef struct ttype {
	int is_init;
	size_t size;
	size_t class_size;
	const char *name;
	struct ttype *parent_type;
	struct tobject_class *class;
} ttype_t;

struct tobject {
	ttype_t *type;
};

struct tobject_class {
	int (*constructor)(void *object);
	int (*destructor)(void *object);
};

TOBJECT_DECLARE_CLASS(tobject, TOBJECT)

static inline tobject_class_t *ttype_get_class(ttype_t *type) {
	return type->class;
}

static inline ttype_t *ttype_get_parent(ttype_t *type) {
	return type->parent_type;
}

void *tobject_new(ttype_t *type);
void tobject_free(void *object);

#endif
