#include "sys/movement_behavior_system.h"
#include "cmp/facing_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"

void MovementBehaviorSystem::update(World &world)
{
	world.view<MovementComponent, MovementBehavior_ConstantDirectionComponent>()
		.each([](auto e,
				 MovementComponent &m,
				 MovementBehavior_ConstantDirectionComponent &t) {
			m.vel = cf_safe_norm(t.dir) * t.speed;
		});

	world
		.view<
			MovementComponent,
			MovementBehavior_FollowTargetComponent,
			TransformComponent>()
		.each([&](auto e,
				  MovementComponent &m,
				  MovementBehavior_FollowTargetComponent &b,
				  TransformComponent &t) {
			auto pos = t.get_world_transform().pos;

			if (!world.valid(b.entity))
			{
				m.vel = cf_v2(0, 0);
				return;
			}

			auto other_pos = world.get<TransformComponent>(b.entity)
								 .get_world_transform()
								 .pos;

			auto dist = other_pos - pos;
			b.dir = cf_safe_norm(other_pos - pos);
			m.vel = b.dir * b.speed;

			FacingComponent *f = world.try_get<FacingComponent>(e);
			if (f)
			{
				f->facing = dist.y < 0 ? Facing::DOWN : Facing::UP;
			}

			if (b.face_target)
			{
				t.set_rotation(atan2(b.dir.y, b.dir.x));
			}
		});
}
