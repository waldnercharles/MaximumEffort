#include "ui.h"
#include "common.h"
#include "log.h"

bool is_mouseover(Aabb aabb)
{
	v2 window_size = cf_v2(cf_app_get_width(), cf_app_get_height());
	v2 mouse_pos = cf_v2(cf_mouse_x(), cf_mouse_y()) - window_size / 2.f;
	return cf_contains_point(aabb, mouse_pos);
}

void UI::begin()
{
	mouse_pressed = UI_MOUSE_NONE;
	mouse_down = UI_MOUSE_NONE;

	if (cf_mouse_just_pressed(CF_MOUSE_BUTTON_LEFT))
		mouse_pressed |= UI_MOUSE_LEFT;
	if (cf_mouse_just_pressed(CF_MOUSE_BUTTON_MIDDLE))
		mouse_pressed |= UI_MOUSE_MIDDLE;
	if (cf_mouse_just_pressed(CF_MOUSE_BUTTON_RIGHT))
		mouse_pressed |= UI_MOUSE_RIGHT;

	if (cf_mouse_down(CF_MOUSE_BUTTON_LEFT))
		mouse_down |= UI_MOUSE_LEFT;
	if (cf_mouse_down(CF_MOUSE_BUTTON_MIDDLE))
		mouse_down |= UI_MOUSE_MIDDLE;
	if (cf_mouse_down(CF_MOUSE_BUTTON_RIGHT))
		mouse_down |= UI_MOUSE_RIGHT;

	elements.clear();
}

void UI::end()
{
	if (!focus_updated)
	{
		focus = 0;
	}

	focus_updated = false;
}

int UI::button(ui_id id, v2 pos, const Sprite sprite, UI_OPTIONS opt)
{
	return button(id, pos, sprite, nullptr, 0, {}, opt);
}

int UI::button(
	ui_id id,
	v2 pos,
	const char *text,
	float font_size,
	CF_Color text_color,
	UI_OPTIONS opt
)
{
	return button(id, pos, {}, text, font_size, text_color, opt);
}

int UI::button(
	ui_id id,
	v2 pos,
	const Sprite sprite,
	const char *text,
	float font_size,
	CF_Color text_color,
	UI_OPTIONS opt
)
{
	// TODO: Get id from a char* or something?

	int action = UI_ACTION_NONE;

	v2 sprite_size = cf_v2(sprite.w, sprite.h);
	v2 text_size = {};
	if (text)
	{
		int w, h;
		cf_app_get_size(&w, &h);

		float percent = font_size / 100.f;
		font_size = cf_min(w, h) * percent;

		cf_push_font("Renogare");
		cf_push_font_size(font_size);
		text_size = {cf_text_width(text), cf_text_height(text)};
		cf_pop_font_size();
		cf_pop_font();
	}

	v2 size = {
		cf_max(sprite_size.x, text_size.x),
		cf_max(sprite_size.y, text_size.y)};

	Aabb aabb = cf_make_aabb_pos_w_h(pos, size.x, size.y);

	update_control(id, aabb, opt);

	// Handle click
	if (mouse_pressed & UI_MOUSE_LEFT && focus == id)
	{
		action |= UI_ACTION_SUBMIT;
	}

	// Draw the UI Element
	if (sprite.name)
	{
		UI_Element element = {};
		element.id = id;
		element.aabb = aabb;
		element.type = UI_ELEMENT_SPRITE;
		element.s.sprite = sprite;
		element.s.pos = pos;

		elements.add(element);
	}

	if (text)
	{


		UI_Element element = {};
		element.id = id;
		element.aabb = aabb;
		element.type = UI_ELEMENT_TEXT;
		element.t.text = sintern(text);
		element.t.pos = pos - cf_v2(text_size.x, -text_size.y) / 2.f;
		element.t.font = sintern("Renogare");
		element.t.font_size = font_size;
		element.t.color = text_color;
		element.t.blur = 0;

		elements.add(element);
	}

	return action;
}

void UI::set_focus(ui_id id)
{
	focus = id;
	focus_updated = true;
}

void UI::update_control(ui_id id, Aabb aabb, UI_OPTIONS opt)
{
	bool mouseover = is_mouseover(aabb);

	if (focus == id)
	{
		focus_updated = true;
	}

	if (opt & UI_NO_INTERACT)
	{
		return;
	}

	if (mouseover && !mouse_down)
	{
		hover = id;
	}

	if (focus == id)
	{
		if (mouse_pressed && !mouseover)
		{
			set_focus(0);
		}
		if (mouse_down && !(opt & UI_HOLD_FOCUS))
		{
			set_focus(0);
		}
	}

	if (hover == id)
	{
		if (mouse_pressed)
		{
			set_focus(id);
		}
		else if (!mouseover)
		{
			hover = 0;
		}
	}
}
void UI::draw()
{
	cf_draw_push_layer(8192);
	for (auto &element : elements)
	{
		cf_draw_push_color(cf_color_green());
		cf_draw_box(element.aabb, 2, 1);
		cf_draw_pop_color();

		switch (element.type)
		{
			case UI_ELEMENT_SPRITE:
			{
				element.s.sprite.update();
				element.s.sprite.draw();
				break;
			}
			case UI_ELEMENT_TEXT:
			{
				cf_draw_push_antialias(true);
				cf_draw_push_antialias_scale(16);

				// TODO: Text with shadow?

				cf_push_font(element.t.font);
				cf_push_font_size(element.t.font_size);
				cf_push_font_blur(element.t.blur);

				if (hover == element.id)
				{
					cf_draw_push_color(cf_color_red());
				}
				else
				{
					cf_draw_push_color(element.t.color);
				}


				cf_draw_text(element.t.text, element.t.pos, -1);

				cf_draw_pop_color();
				cf_pop_font_blur();
				cf_pop_font_size();
				cf_pop_font();

				cf_draw_pop_antialias_scale();
				cf_draw_pop_antialias();
				break;
			}
		}
	}
	cf_draw_pop_layer();
}
