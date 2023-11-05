#include "player.h"

#include "common.h"
#include "fmt/format.h"

using namespace Cute;

void Player::update(float dt)
{
	input_dir = {};

	if (key_down(KEY_W) || key_down(KEY_UP))
	{
		input_dir.y += 1;
	}
	if (key_down(KEY_A) || key_down(KEY_LEFT))
	{
		input_dir.x -= 1;
	}
	if (key_down(KEY_S) || key_down(KEY_DOWN))
	{
		input_dir.y -= 1;
	}
	if (key_down(KEY_D) || key_down(KEY_RIGHT))
	{
		input_dir.x += 1;
	}

	if (input_dir.x != 0)
	{
		facing = input_dir.x;
	}

	f32 dir_length = cf_len(input_dir);
	if (dir_length > 0.0f)
	{
		// Normalize
		input_dir = input_dir / dir_length;

		vel = input_dir * dt * speed;
		v2 new_pos = pos + vel;

		// TODO: Collision detection / resolution

		pos = new_pos;
	}
}

void Player::draw()
{
	sprite.transform.p = pos;

	auto facing_anim = facing > 0 ? "right" : "left";
	auto action_anim = input_dir.x != 0 || input_dir.y != 0 ? "walk" : "idle";

	auto anim =
		fmt::vformat("{}-{}", fmt::make_format_args(action_anim, facing_anim));

	if (!sprite.is_playing(anim.c_str()))
	{
		sprite.play(anim.c_str());
	}

	draw_push_layer(42);

	sprite.update();
	sprite.draw();

	draw_pop_layer();
}
