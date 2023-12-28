#include "sys/movement_behavior_system.h"
#include "cmp/facing_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"


void MovementBehaviorSystem::update(World &world)
{
	world.view<C_Movement, C_RayMovement>().each([](C_Movement &m,
													C_RayMovement &r) {
		r.speed -= r.speed * r.damping * CF_DELTA_TIME_FIXED;
		m.vel = cf_safe_norm(r.dir) * r.speed;
	});

	world.view<C_Movement, MovementBehavior_ConstantDirectionComponent>().each(
		[](auto e, C_Movement &m, MovementBehavior_ConstantDirectionComponent &t
		) {
			m.vel = cf_safe_norm(t.dir) * t.speed * CF_DELTA_TIME_FIXED;
		}
	);

	world
		.view<
			C_Movement,
			MovementBehavior_FollowTargetComponent,
			C_Stats,
			C_Transform>()
		.each([&](auto e,
				  C_Movement &m,
				  MovementBehavior_FollowTargetComponent &b,
				  C_Stats &s,
				  C_Transform &t) {
			auto stats = s.get_stats();
			auto pos = t.get_world_transform().pos;

			if (!world.valid(b.entity))
			{
				m.vel = cf_v2(0, 0);
				return;
			}

			auto other_pos =
				world.get<C_Transform>(b.entity).get_world_transform().pos;

			auto dist = other_pos - pos;
			b.dir = cf_safe_norm(other_pos - pos);
			m.vel = b.dir * stats[Stat::speed];

			C_Facing *f = world.try_get<C_Facing>(e);
			if (f)
			{
				f->facing_x = cf_sign_int((int)dist.x);
				f->facing_y = cf_sign_int((int)dist.y);
			}

			if (b.face_target)
			{
				t.set_rotation(atan2(b.dir.y, b.dir.x));
			}
		});
}
