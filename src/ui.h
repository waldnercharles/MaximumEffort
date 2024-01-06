#pragma once
#include "common.h"

typedef u32 ui_id;

enum UI_OPTIONS
{
	//	UI_ALIGN_LEFT = 1 << 0,
	//	UI_ALIGN_CENTER = 1 << 1,
	//	UI_ALIGN_RIGHT = 1 << 2,
	UI_NO_INTERACT = 1 << 3,
	UI_HOLD_FOCUS = 1 << 4,

	UI_OPTIONS_DEFAULT = UI_HOLD_FOCUS,
};

enum UI_ACTION : int
{
	UI_ACTION_NONE = 0,
	UI_ACTION_ACTIVE = 1 << 0,
	UI_ACTION_SUBMIT = 1 << 1,
	//	UI_ACTION_CHANGE = 1 << 2,
};

enum UI_MOUSE_BUTTON
{
	UI_MOUSE_NONE = 0,
	UI_MOUSE_LEFT = 1 << 0,
	UI_MOUSE_MIDDLE = 1 << 1,
	UI_MOUSE_RIGHT = 1 << 2
};

enum UI_ELEMENT_TYPE
{
	UI_ELEMENT_SPRITE,
	UI_ELEMENT_TEXT
};

struct UI_Element
{
	ui_id id;
	Aabb aabb;

	UI_ELEMENT_TYPE type;

	union
	{
		struct
		{
			Sprite sprite;
			v2 pos;
		} s;

		struct
		{
			const char *text;
			v2 pos;

			const char *font;
			float font_size;

			int blur;

			CF_Color color;
		} t;
	};
};

struct UI
{
	void begin();

	int button(
		ui_id id,
		v2 pos,
		const Sprite sprite,
		UI_OPTIONS opt = UI_OPTIONS_DEFAULT
	);

	int button(
		ui_id id,
		v2 pos,
		const char *text,
		float font_size,
		CF_Color text_color,
		UI_OPTIONS opt = UI_OPTIONS_DEFAULT
	);

	int button(
		ui_id id,
		v2 pos,
		const Sprite sprite,
		const char *text,
		float font_size,
		CF_Color text_color,
		UI_OPTIONS opt = UI_OPTIONS_DEFAULT
	);

	void end();

	void draw();

private:
	void set_focus(ui_id id);
	void update_control(ui_id id, Aabb aabb, UI_OPTIONS opt);

	ui_id focus = 0;
	bool focus_updated = false;

	ui_id hover = 0;

	int mouse_down = UI_MOUSE_NONE;
	int mouse_pressed = UI_MOUSE_NONE;

	Array<UI_Element> elements;
};
