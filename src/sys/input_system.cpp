#include "sys/input_system.h"
#include "cmp/c_input.h"
#include "cmp/c_movement.h"
#include "cmp/c_player.h"

#include <cute.h>

using namespace Cute;

void input_system(entt::registry &reg, float dt)
{
	reg.view<C_Input, C_Movement>().each(
		[&reg](auto e, C_Input &i, C_Movement &m) {
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

			C_Player *p = reg.try_get<C_Player>(e);
			if (p)
			{
				p->facing = i.input_dir.x;
			}

			m.vel = cf_safe_norm(i.input_dir) * i.speed;
		}
	);
}