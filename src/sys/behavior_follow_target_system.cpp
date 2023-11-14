#include "..\..\include\sys\behavior_follow_target_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_follow_target.h"
#include "cmp/c_transform.h"

using namespace Cute;

void add_behavior_follow_target_system(flecs::world *world)
{
	world
		->system<
			C_Behavior_FollowTarget,
			C_Movement,
			const C_WorldTransform,
			C_LocalTransform>("behavior_follow_target")
		.each([](flecs::iter &it,
				 size_t,
				 C_Behavior_FollowTarget &b,
				 C_Movement &m,
				 const C_WorldTransform &w,
				 C_LocalTransform &l) {
			const C_WorldTransform *wt_world = b.target.get<C_WorldTransform>();

			b.dir = safe_norm(wt_world->pos - w.pos);
			m.vel = b.dir * b.speed;
			if (b.face_target)
			{
				l.angle = atan2(b.dir.y, b.dir.x);
			}
		});
}
