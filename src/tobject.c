#include <tobject.h>
#include <stdlib.h>

TOBJECT_DEFINE_CLASS(tobject, TOBJECT, NULL)

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
