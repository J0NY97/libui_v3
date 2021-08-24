#ifndef UI_ENUM_H
# define UI_ENUM_H

enum	e_element_colors
{
	UI_COLOR_BLUEISH = 0xff95D7AE,
	UI_COLOR_BLUEISH_DARKER = 0xff7BAE7F,
	UI_COLOR_BLUEISH_DARKEST = 0xff73956F,

	UI_COLOR_ORANGEISH = 0xffF4D35E, 
	UI_COLOR_ORANGEISH_DARKER = 0xffEE964B,
	UI_COLOR_ORANGEISH_DARKEST = 0xffF95738
};

enum	e_element_states
{
	UI_STATE_DEFAULT = 0,
	UI_STATE_HOVER,
	UI_STATE_CLICK,
	UI_STATE_AMOUNT
};

enum	e_element_types
{
	UI_TYPE_NONE = -1,
	UI_TYPE_ELEMENT = 0,
	UI_TYPE_LABEL,
	UI_TYPE_BUTTON,
	UI_TYPE_MENU,
	UI_TYPE_DROPDOWN,
	UI_TYPE_INPUT,
	UI_TYPE_SLIDER,
	UI_TYPE_WINDOW,
	UI_TYPE_AMOUNT
};

#endif
