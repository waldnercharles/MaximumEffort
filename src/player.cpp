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

		vel = input_dir * speed * dt;
		v2 new_pos = pos + vel;

		// TODO: Collision detection / resolution

		pos = new_pos;
	}
}

void Player::draw()
{
	static const char *anims[4] =
		{"idle-left", "idle-right", "walk-left", "walk-right"};

	sprite.transform.p = pos;

	int action_anim = input_dir.x != 0 || input_dir.y != 0 ? 2 : 0;
	int facing_anim = facing > 0 ? 1 : 0;

	int anim_index = action_anim + facing_anim;

	const char *anim = anims[anim_index];

	if (!sprite.is_playing(anim))
	{
		sprite.play(anim);
	}

	draw_push_layer(42);

	sprite.update();
	sprite.draw();

	draw_pop_layer();
}
