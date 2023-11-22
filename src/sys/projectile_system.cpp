#include "sys/projectile_system.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/projectile_component.h"
#include "cmp/transform_component.h"
#include "common.h"
#include "events/hit_event.h"

ProjectileSystem::ProjectileSystem(
	EventBus &event_bus,
	AabbGrid<Entity> &enemy_grid
)
	: event_bus(event_bus),
	  enemy_grid(enemy_grid)
{
}

void ProjectileSystem::update(World &w)
{
	auto view = w.view<
		ProjectileComponent,
		TransformComponent,
		HurtboxComponent>();

	for (auto proj_entity : view)
	{
		auto proj = view.get<ProjectileComponent>(proj_entity);
		auto &proj_transform = view.get<TransformComponent>(proj_entity);
		auto proj_circle = view.get<HurtboxComponent>(proj_entity).circle;
		proj_circle.p += proj_transform.get_world_transform().pos;

		Aabb proj_aabb = cf_make_aabb(
			proj_circle.p - cf_v2(proj_circle.r, proj_circle.r),
			proj_circle.p + cf_v2(proj_circle.r, proj_circle.r)
		);

		enemy_grid.query(proj_aabb, [&](Entity enemy_entity) {
			auto &enemy_transform = w.get<TransformComponent>(enemy_entity);
			auto enemy_circle = w.get<HitboxComponent>(enemy_entity).circle;
			enemy_circle.p += enemy_transform.get_world_transform().pos;

			if (cf_circle_to_circle(proj_circle, enemy_circle))
			{
				event_bus.emit<HitEvent>(proj_entity, enemy_entity);
				if (!proj.piercing)
				{
					w.emplace<FrameDestroyComponent>(proj_entity);
					return false;
				}
			}

			return true;
		});
	}
}
