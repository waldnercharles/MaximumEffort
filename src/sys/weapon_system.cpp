#include "sys/weapon_system.h"
#include "cmp/movement_component.h"
#include "cmp/weapon_component.h"
#include "game.h"
#include "projectile_factories.h"

#include <cute.h>

using namespace Cute;

void weapon_system(entt::registry &reg, float dt)
{
	reg.view<WeaponComponent>().each([&](auto weapon_entity,
										 WeaponComponent &w) {
		if (w.parent == entt::null)
		{
			return;
		}

		auto *parent_movement = reg.try_get<MovementComponent>(w.parent);
		if (parent_movement == nullptr)
		{
			return;
		}

		if (on_interval(w.rate))
		{
			// TODO: Separate this into some sort of ProjectileTargetComponent
			float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
			v2 dir = V2(cosf(angle), sinf(angle));

			if (w.weapon_type == WEAPON_BOOMERANG)
			{
				make_projectile_boomerang(reg, parent_movement->pos, dir);
			}
		}
	});
}
