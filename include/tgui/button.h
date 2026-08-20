#ifndef TGUI_BUTTON_H
#define TGUI_BUTTON_H

#include "widget.h"

TOBJECT_DECLARE_SIMPLE_CLASS(tgui_button, TGUI_BUTTON, tgui_widget)

/**
 * @struct tgui_button
 * A button is a widget that display a visual button with text, icon or any child widget on which the user can click.
 */
struct tgui_button {
	tgui_widget_t widget;
	tgui_widget_t *child;
};

/**
 * @relates tgui_button
 * @brief create a new button
 * @return the new button
 */
tgui_button_t *tgui_button_new(void);

/**
 * @relates tgui_button
 * @brief set the text of a button
 * @param button the button to set the text of
 * @param text the text to set
 * @note this destroy any previous non label child widget
 */
void tgui_button_set_text(tgui_button_t *button, const char *text);

/**
 * @relates tgui_button
 * @brief set the icon of a button
 * @param button the button to set the icon of
 * @param icon the name of the icon to set
 * @note this destroy any previous non icon child widget
 */
void tgui_button_set_icon(tgui_button_t *button, const char *icon);

/**
 * @relates tgui_button
 * @brief set the child widget of a button
 * @param button the button to set the child widget of
 * @param child the child widget to set
 * @note this destroy any previous child widget
 */
void tgui_button_set_child(tgui_button_t *button, tgui_widget_t *child);

/**
 * @relates tgui_button
 * @brief get the text of a button
 * @param button the button to get the text of
 * @return the text of the button or NULL
 */
const char *tgui_button_get_text(tgui_button_t *button);

/**
 * @relates tgui_button
 * @brief get the icon name of a button
 * @param button the button to get the icon of
 * @return the icon name of the button or NULL
 */
const char *tgui_button_get_icon(tgui_button_t *button);

/**
 * @relates tgui_button
 * @brief get the child widget of a button
 * @param button the button to get the child widget of
 * @return the child widget of the button or NULL
 */
tgui_widget_t *tgui_button_get_child(tgui_button_t *button);

#endif
