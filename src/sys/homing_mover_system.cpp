#include "homing_mover_system.h"
#include "cmp/homing_mover_component.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"

void HomingMoverSystem::update(World &world)
{
	world.view<C_Transform, C_Movement, C_HomingMover>().each(
		[&](C_Transform &t, C_Movement &movement, C_HomingMover &mover) {
			if (!world.valid(mover.target))
			{
				movement.vel = {};
				return;
			}

			auto target_pos =
				world.get<C_Transform>(mover.target).get_world_transform().pos;

			auto pos = t.get_world_transform().pos;
			auto dir = cf_safe_norm(target_pos - pos);

			mover.speed = mover.speed + mover.acceleration * DELTA_TIME_FIXED;
			movement.vel = dir * mover.speed;
		}
	);
}
