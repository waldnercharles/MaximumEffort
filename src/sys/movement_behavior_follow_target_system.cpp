#include "sys/movement_behavior_follow_target_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_follow_target.h"
#include "cmp/c_transform.h"

using namespace Cute;

void movement_behavior_follow_target_system(entt::registry &reg, float dt)
{
	//	reg.view<C_Movement, C_MovementBehavior_FollowTarget, C_WorldTransform>()
	//		.each([&](auto e,
	//				  C_Movement &m,
	//				  C_MovementBehavior_FollowTarget &b,
	//				  C_WorldTransform &t) {
	//			auto pos = t.pos;
	//			//			auto pos = t.get_global_transform().pos;
	//
	//			if (!reg.valid(b.entity))
	//			{
	//				m.vel = V2(0, 0);
	//				return;
	//			}
	//
	//			auto other_pos =
	//				reg.get<C_WorldTransform>(b.entity)
	//					//								 .get_global_transform()
	//					.pos;
	//
	//			b.dir = safe_norm(other_pos - pos);
	//			m.vel = b.dir * b.speed;
	//
	//			if (b.face_target)
	//			{
	//				t.angle = atan2(b.dir.y, b.dir.x);
	//			}
	//		});
}
