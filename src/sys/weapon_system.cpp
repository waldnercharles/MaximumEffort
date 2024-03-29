#include "sys/weapon_system.h"
#include "cmp/bullet_emitter_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"
#include "game_timer.h"
#include "log.h"
#include "prefabs/projectile_prefab.h"

#include <cute.h>

v2 get_random_dir(Rnd &rnd)
{
	float angle = cf_rnd_next_range_float(&rnd, -PI, PI);
	return cf_v2(cosf(angle), sinf(angle));
}

bool try_get_closest_enemy_dir(
	World &world,
	AabbGrid<Entity> &enemy_aabb_grid,
	Circle circle,
	v2 *dir
)
{
	Entity closest_entity = ECS_NULL;
	float closest_dist;

	v2 source_pos = circle.p;
	float radius = circle.r;

	auto aabb =
		cf_make_aabb_center_half_extents(circle.p, {circle.r, circle.r});

	enemy_aabb_grid.query(aabb, [&](Entity enemy) {
		if (!world.valid(enemy))
		{
			return true;
		}

		auto enemy_pos =
			world.get<C_Transform>(enemy).get_world_transform().pos;

		float dist = cf_distance(source_pos, enemy_pos);

		if (dist > radius)
		{
			return true;
		}

		if (closest_entity == ECS_NULL || dist < closest_dist)
		{
			closest_dist = dist;
			closest_entity = enemy;
			*dir = enemy_pos - source_pos;
		}

		return true;
	});

	return closest_entity != ECS_NULL;
}

WeaponSystem::WeaponSystem(
	AabbGrid<Entity> &enemy_aabb_grid,
	GameTimer &game_timer
)
	: rnd(cf_rnd_seed((u64)time(nullptr))),
	  enemy_aabb_grid(enemy_aabb_grid),
	  game_timer(game_timer)
{
}

void WeaponSystem::update(World &world)
{
	world.view<C_BulletEmitter, C_Transform>().each([&](auto weapon_entity,
														C_BulletEmitter &weapon,
														C_Transform &transform
													) {
		if (game_timer.on_interval(weapon.interval))
		{
			v2 pos = transform.get_world_transform().pos;
			v2 dir = {};

			bool has_target = false;

			auto targeting_circle = cf_make_circle(pos, weapon.emission_radius);

			has_target = try_get_closest_enemy_dir(
				world,
				enemy_aabb_grid,
				targeting_circle,
				&dir
			);


			if (!has_target)
			{
				return;
			}

			auto stats = world.get<C_Stats>(weapon.owner);

			int projectile_amount = 1; //stats.get_stats().projectile_amount;
			if (projectile_amount == 0)
			{
				return;
			}

			dir = cf_safe_norm(dir);
			auto angle = atan2(dir.y, dir.x);
			auto angle_delta = 0.f;

			if (projectile_amount > 1)
			{
				auto arc = PI * 0.125f;
				angle_delta = arc * 2 / (projectile_amount - 1);

				angle -= arc;
			}

			for (auto i = 0; i < projectile_amount; i++)
			{
				prefabs::Projectile::create(
					world,
					weapon.owner,
					pos,
					angle,
					next_attack_id
				);

				angle += angle_delta;
			}

			next_attack_id++;
		}
	});
}
