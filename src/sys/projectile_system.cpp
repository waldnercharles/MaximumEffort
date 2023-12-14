#include "sys/projectile_system.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/hit_component.h"
#include "cmp/hit_immunity_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/projectile_component.h"
#include "cmp/transform_component.h"
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
	auto view = w.view<C_Projectile, C_Transform, C_Hurtbox>();

	for (auto proj : view)
	{
		auto &proj_transform = view.get<C_Transform>(proj);
		const auto &proj_hurtbox = view.get<C_Hurtbox>(proj);
		auto proj_circle = proj_hurtbox.circle;
		proj_circle.p += proj_transform.get_world_transform().pos;

		Aabb proj_aabb = cf_make_aabb(
			proj_circle.p - cf_v2(proj_circle.r, proj_circle.r),
			proj_circle.p + cf_v2(proj_circle.r, proj_circle.r)
		);

		enemy_grid.query(proj_aabb, [&](Entity enemy) {
			auto &enemy_transform = w.get<C_Transform>(enemy);
			auto &enemy_hitbox = w.get<C_Hitbox>(enemy);

			auto enemy_circle = enemy_hitbox.circle;
			enemy_circle.p += enemy_transform.get_world_transform().pos;

			if (cf_circle_to_circle(proj_circle, enemy_circle))
			{
				auto &hit = w.get<C_Hit>(proj);
				auto *immunity = w.try_get<C_HitImmunity>(enemy);

				if (immunity != nullptr)
				{
					for (int i = 0; i < immunity->count; i++)
					{
						if (immunity->hits[i].id == hit.id)
						{
							return true;
						}
					}
				}

				event_bus.emit<HitEvent>(proj, enemy);
				if (!view.get<C_Projectile>(proj).piercing)
				{
					w.emplace<C_FrameDestroy>(proj);
					return false;
				}
			}

			return true;
		});
	}
}
