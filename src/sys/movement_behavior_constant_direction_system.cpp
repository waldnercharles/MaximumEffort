#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_constant_direction.h"
#include "sys/movement_behavior_follow_target_system.h"

#include <cute.h>

using namespace Cute;

void movement_behavior_constant_direction_system(entt::registry &reg, float dt)
{
	reg.view<C_Movement, C_MovementBehavior_ConstantDirection>().each(
		[&](auto e, C_Movement &m, C_MovementBehavior_ConstantDirection &t) {
			m.vel = safe_norm(t.dir) * t.speed;
		}
	);
}
