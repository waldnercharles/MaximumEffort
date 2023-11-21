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

	for (auto proj : view)
	{
		bool piercing = view.get<ProjectileComponent>(proj).piercing;
		auto &proj_transform = view.get<TransformComponent>(proj);
		auto proj_circle = view.get<HurtboxComponent>(proj).circle;
		proj_circle.p += proj_transform.get_world_transform().pos;

		Aabb proj_aabb = cf_make_aabb(
			proj_circle.p - cf_v2(proj_circle.r, proj_circle.r),
			proj_circle.p + cf_v2(proj_circle.r, proj_circle.r)
		);

		enemy_grid.query(proj_aabb, [&](Entity enemy) {
			auto &enemy_transform = w.get<TransformComponent>(enemy);
			auto enemy_circle = w.get<HitboxComponent>(enemy).circle;
			enemy_circle.p += enemy_transform.get_world_transform().pos;

			if (cf_circle_to_circle(proj_circle, enemy_circle))
			{
				event_bus.emit<HitEvent>(proj, enemy);
				if (!piercing)
				{
					w.emplace<FrameDestroyComponent>(proj);
					return false;
				}
			}

			return true;
		});
	}
}
