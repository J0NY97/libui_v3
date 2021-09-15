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
	UI_TYPE_CHECKBOX,
	UI_TYPE_RADIO,
	UI_TYPE_TAB,
	UI_TYPE_WINDOW,
	UI_TYPE_AMOUNT
};

/*
 * NOTE:
 * LEFT/RIGHT can be combined with TOP/BOT
*/
enum e_ui_text_align
{
	UI_TEXT_ALIGN_NONE		= 0x00000000,
	UI_TEXT_ALIGN_LEFT		= 0x00000001,
	UI_TEXT_ALIGN_RIGHT		= 0x00000010,
	UI_TEXT_ALIGN_TOP		= 0x00000100,
	UI_TEXT_ALIGN_BOT		= 0x00001000,
	UI_TEXT_ALIGN_CENTER	= 0x00010000
};

/*
 * These are flags, for a window, that can be changed after the fact.
 *
 * NOTE:
 * FULLSCREEN toggles when set (if SDL_FALSE makes SDL_TRUE and vice versa)
*/
enum e_window_flags
{
	UI_WINDOW_HIDE					= 0x00000001,
	UI_WINDOW_SHOW					= 0x00000010,
	UI_WINDOW_MAXIMIZE				= 0x00000100,
	UI_WINDOW_MINIMIZE				= 0x00001000,
	UI_WINDOW_FULLSCREEN			= 0x00010000,
	UI_WINDOW_FULLSCREEN_WINDOWED	= 0x00100000,
	UI_WINDOW_GRAB					= 0x01000000,
	UI_WINDOW_RESIZEABLE			= 0x10000000
};

#endif
