#include "sys/movement_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_transform.h"

#include <cute.h>
#include <flecs.h>

using namespace Cute;

void add_movement_system(flecs::world *world)
{
	world->system<C_LocalTransform, C_Movement>("movement")
		.each([](C_LocalTransform &t, const C_Movement &m) {
			t.pos += m.vel * DELTA_TIME_FIXED;
			t.angle = fmodf(
				t.angle + m.angular_vel * DELTA_TIME_FIXED,
				CF_PI * 2.f
			);
		});
}