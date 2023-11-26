#include "sys/input_system.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"

#include <cute.h>

void InputSystem::update(World &world)
{
	world.view<InputComponent, MovementComponent>().each(
		[&world](auto e, InputComponent &i, MovementComponent &m) {
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

			FacingComponent *f = world.try_get<FacingComponent>(e);
			if (f)
			{
				//				f->facing = i.input_dir.y > 0 ? Facing::UP : Facing::DOWN;
				f->facing = i.input_dir.x > 0 ? Facing::RIGHT : Facing::LEFT;
			}

			m.vel = cf_safe_norm(i.input_dir) * i.speed;
		}
	);
}
