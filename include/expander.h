#ifndef TGUI_EXPANDER_H
#define TGUI_EXPANDER_H

#include "widget.h"
#include "box.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_expander, TGUI_EXPANDER, tgui_box)

struct tgui_expander {
	tgui_box_t box;
	tgui_widget_t *title;
	tgui_widget_t *child;
	int expanded;
};

tgui_expander_t *tgui_expander_new(void);
void tgui_expander_set_title_text(tgui_expander_t *expander, const char *text);
void tgui_expander_set_title_child(tgui_expander_t *expander, tgui_widget_t *child);
void tgui_expander_set_child(tgui_expander_t *expander, tgui_widget_t *child);
void tgui_expander_expand(tgui_expander_t *expander);
void tgui_expander_unexpand(tgui_expander_t *expander);
int tgui_expander_is_expanded(tgui_expander_t *expander);

#endif
