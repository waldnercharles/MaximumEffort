#include "cmp/movement.h"
#include "cmp/movement_behavior_constant_direction.h"
#include "sys/movement_behavior_follow_target_system.h"

#include <cute.h>


void movement_behavior_constant_direction_system(World &world, float dt)
{
	world.view<Movement, MovementBehavior_ConstantDirection>().each(
		[&](auto e, Movement &m, MovementBehavior_ConstantDirection &t) {
			m.vel = cf_safe_norm(t.dir) * t.speed;
		}
	);
}
