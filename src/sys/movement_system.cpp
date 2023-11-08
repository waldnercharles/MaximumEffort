#include "sys/movement_system.h"
#include "cmp/follow_target_behavior_component.h"
#include "cmp/movement_component.h"
#include "cmp/projectile_component.h"
#include "cmp/scene_node_component.h"
#include "cmp/target_direction_behavior_component.h"
#include "log.h"

#include <cute.h>

using namespace Cute;

void movement_system(entt::registry &reg, float dt)
{
	reg.view<FollowTargetBehaviorComponent, MovementComponent>().each(
		[&](auto e, FollowTargetBehaviorComponent &t, MovementComponent &m) {
			m.vel = safe_norm(t.dir) * t.speed;
		}
	);

	reg.view<TargetDirectionBehaviorComponent, MovementComponent>().each(
		[&](auto e, TargetDirectionBehaviorComponent &t, MovementComponent &m) {
			m.vel = safe_norm(t.dir) * t.speed;
		}
	);

	reg.view<MovementComponent, SceneNodeComponent>().each(
		[&](auto e, MovementComponent &m, SceneNodeComponent &s) {
			auto &t = s.get_local_transform();

			s.set_local_transform(
				{t.pos + m.vel * dt,
				 fmodf(t.angle + m.angular_vel * dt, CF_PI * 2.f)}
			);
		}
	);
}