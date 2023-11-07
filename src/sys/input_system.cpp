#include "sys/input_system.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"

#include <cute.h>

using namespace Cute;

void input_system(entt::registry &reg, float dt)
{
	reg.view<InputComponent, MovementComponent>().each(
		[&reg](auto e, InputComponent &i, MovementComponent &m) {
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

			PlayerComponent *p = reg.try_get<PlayerComponent>(e);
			if (p)
			{
				p->facing = i.input_dir.x;
			}

			m.dir = cf_safe_norm(i.input_dir);
		}
	);
}