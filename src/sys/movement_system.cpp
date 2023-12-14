#include "sys/movement_system.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"

#include <cute.h>

void MovementSystem::update(World &world)
{
	world.view<C_Movement, C_Transform>().each(
		[](auto e, C_Movement &m, C_Transform &transform) {
			const Transform &local_transform = transform.get_local_transform();

			transform = local_transform;

			transform.set_transform(
				{local_transform.pos + m.vel * DELTA_TIME_FIXED,
				 fmodf(
					 local_transform.angle + m.angular_vel * DELTA_TIME_FIXED,
					 PI * 2.f
				 )}
			);
		}
	);
}
