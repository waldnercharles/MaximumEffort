#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_constant_direction.h"

#include <cute.h>
#include <flecs.h>

using namespace Cute;

void add_behavior_constant_direction_system(flecs::world *world)
{
	auto s = world->system<const C_Behavior_ConstantDirection, C_Movement>(
		"BehaviorConstantDirection"
	);

	s.each([](const C_Behavior_ConstantDirection &b, C_Movement &m) {
		m.vel = safe_norm(b.dir) * b.speed;
	});
}
