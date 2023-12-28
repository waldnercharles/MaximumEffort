#include "sys/input_system.h"
#include "aabb_grid.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"

#include <cute.h>

void InputSystem::update(World &world)
{
	world.view<C_Player, C_Input, C_Movement, C_Stats>().each(
		[&world](auto e, C_Input &i, C_Movement &m, C_Stats &s) {
			i.input_dir = {};

			if (cf_key_down(CF_KEY_W) || cf_key_down(CF_KEY_UP))
			{
				i.input_dir.y += 1;
			}
			if (cf_key_down(CF_KEY_A) || cf_key_down(CF_KEY_LEFT))
			{
				i.input_dir.x -= 1;
			}
			if (cf_key_down(CF_KEY_S) || cf_key_down(CF_KEY_DOWN))
			{
				i.input_dir.y -= 1;
			}
			if (cf_key_down(CF_KEY_D) || cf_key_down(CF_KEY_RIGHT))
			{
				i.input_dir.x += 1;
			}

			C_Facing *f = world.try_get<C_Facing>(e);
			if (f)
			{
				if (i.input_dir.x != 0)
				{
					f->facing_x = cf_sign_int((int)i.input_dir.x);
				}

				if (i.input_dir.y != 0)
				{
					f->facing_y = cf_sign_int((int)i.input_dir.y);
				}
			}

			float speed = s.get(Stat::speed);
			m.vel = cf_safe_norm(i.input_dir) * speed;
		}
	);
}
