#include "sys/input_system.h"
#include "cmp/c_input.h"
#include "cmp/c_movement.h"
#include "cmp/c_player.h"

#include <cute.h>

using namespace Cute;

void add_input_system(flecs::world *world)
{
	world->system<C_Input, C_Movement, C_Player *>("input").each(
		[](C_Input &i, C_Movement &m, C_Player *p) {
			i.input_dir = {};

			if (key_down(KEY_W) || key_down(KEY_UP))
			{
				i.input_dir.y += 1;
			}
			if (key_down(KEY_A) || key_down(KEY_LEFT))
			{
				i.input_dir.x -= 1;
			}
			if (key_down(KEY_S) || key_down(KEY_DOWN))
			{
				i.input_dir.y -= 1;
			}
			if (key_down(KEY_D) || key_down(KEY_RIGHT))
			{
				i.input_dir.x += 1;
			}

			if (p)
			{
				p->facing = i.input_dir.x;
			}

			m.vel = cf_safe_norm(i.input_dir) * i.speed;
		}
	);
}