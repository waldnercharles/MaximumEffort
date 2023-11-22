#include "sys/weapon_system.h"
#include "cmp/transform_component.h"
#include "cmp/weapon_component.h"
#include "prefabs/projectile_boomerang_prefab.h"

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

	auto aabb = cf_make_aabb_center_half_extents(
		circle.p,
		{circle.r, circle.r}
	);

	enemy_aabb_grid.query(aabb, [&](Entity enemy) {
		if (!world.valid(enemy))
		{
			return true;
		}

		auto enemy_pos = world.get<TransformComponent>(enemy)
							 .get_world_transform()
							 .pos;

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

WeaponSystem::WeaponSystem(AabbGrid<Entity> &enemy_aabb_grid)
	: rnd(cf_rnd_seed((u64)time(nullptr))),
	  enemy_aabb_grid(enemy_aabb_grid)
{
}

void WeaponSystem::update(World &world)
{
	world.view<WeaponComponent, TransformComponent>().each(
		[&](auto weapon_entity,
			WeaponComponent &weapon,
			TransformComponent &scene_node) {
			if (cf_on_interval(weapon.rate, 0))
			{
				v2 pos = scene_node.get_world_transform().pos;
				v2 dir = {};

				bool has_target = false;
				if (weapon.target_type == TARGET_RANDOM_DIR)
				{
					has_target = true;
					dir = get_random_dir(rnd);
				}
				else if (weapon.target_type == TARGET_CLOSEST_ENEMY)
				{
					auto targeting_circle = cf_make_circle(
						pos,
						weapon.targeting_radius
					);

					has_target = try_get_closest_enemy_dir(
						world,
						enemy_aabb_grid,
						targeting_circle,
						&dir
					);
				}

				if (!has_target)
				{
					return;
				}

				Entity e = ECS_NULL;
				if (weapon.weapon_type == WEAPON_BOOMERANG)
				{
					e = prefabs::ProjectileBoomerang::create(
						world,
						pos,
						dir,
						0
					);
				}

				if (e == ECS_NULL)
				{
					return;
				}
			}
		}
	);
}
