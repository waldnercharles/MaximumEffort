#include "sys/movement_behavior_follow_target_system.h"
#include "cmp/movement.h"
#include "cmp/movement_behavior_follow_target.h"
#include "cmp/transform.h"

void movement_behavior_follow_target_system(World &world, float dt)
{
	world.view<Movement, MovementBehavior_FollowTarget, Transform>().each(
		[&](auto e, Movement &m, MovementBehavior_FollowTarget &b, Transform &t
		) {
			auto pos = t.get_global_transform().pos;

			if (!world.valid(b.entity))
			{
				m.vel = cf_v2(0, 0);
				return;
			}

			auto other_pos = world.get<Transform>(b.entity)
								 .get_global_transform()
								 .pos;

			b.dir = cf_safe_norm(other_pos - pos);
			m.vel = b.dir * b.speed;

			if (b.face_target)
			{
				t.set_rotation(atan2(b.dir.y, b.dir.x));
			}
		}
	);
}
