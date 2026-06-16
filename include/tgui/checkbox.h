#ifndef TGUI_CHECKBOX_H
#define TGUI_CHECKBOX_H

#include "togglebutton.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_checkbox, TGUI_CHECKBOX, tgui_toggle_button)

struct tgui_checkbox {
	tgui_toggle_button_t toggle_button;
};

tgui_checkbox_t *tgui_checkbox_new(void);

static inline void tgui_checkbox_set(tgui_checkbox_t *checkbox, int value) {
	tgui_toggle_button_set_active(TGUI_TOGGLE_BUTTON_CAST(checkbox), value);
}

static inline int tgui_checkbox_get(tgui_checkbox_t *checkbox) {
	return tgui_toggle_button_get_active(TGUI_TOGGLE_BUTTON_CAST(checkbox));
}

#endif
