#include "sys/movement_system.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"

#include <cute.h>

void MovementSystem::update(World &world)
{
	world.view<MovementComponent, TransformComponent>().each(
		[](auto e, MovementComponent &m, TransformComponent &transform) {
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
