#include "sys/weapon_system.h"
#include "cmp/transform.h"
#include "cmp/weapon.h"
#include "game.h"
#include "projectile_factories.h"

#include <cute.h>

v2 get_random_dir()
{
	float angle = cf_rnd_next_range_float(&game.rnd, -PI, PI);
	return cf_v2(cosf(angle), sinf(angle));
}

struct TargetClosestEnemyQueryData
{
	v2 player_pos = {};
	float radius = {};

	Entity entity = entt::null;

	float dist = {};
	v2 dir = {};
};

bool on_closest_enemy_query_hit(
	//	AabbGridNode,
	Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &world = game.world;

	Entity enemy = (Entity)(uint64_t)leaf_udata;
	if (!world.valid(enemy))
	{
		return true;
	}

	auto *query_data = (TargetClosestEnemyQueryData *)fn_udata;

	auto player_pos = query_data->player_pos;
	auto enemy_pos = world.get<Transform>(enemy).get_global_transform().pos;

	float dist = cf_distance(player_pos, enemy_pos);

	if (dist > query_data->radius)
	{
		return true;
	}

	if (query_data->entity == ECS_NULL || dist < query_data->dist)
	{
		query_data->dist = dist;
		query_data->entity = enemy;
		query_data->dir = enemy_pos - player_pos;
	}

	return true;
}

bool try_get_closest_enemy_dir(Circle circle, v2 *dir)
{
	TargetClosestEnemyQueryData data;
	data.entity = ECS_NULL;
	data.player_pos = circle.p;
	data.radius = circle.r;

	auto aabb = cf_make_aabb_center_half_extents(
		circle.p,
		{circle.r, circle.r}
	);
	//	aabb_grid_query(game.enemy_grid, aabb, on_closest_enemy_query_hit, &data);

	*dir = data.dir;

	return data.entity != ECS_NULL;
}

void weapon_system(World &world, float dt)
{
	world.view<Weapon, Transform>().each([&](auto weapon_entity,
											 Weapon &weapon,
											 Transform &scene_node) {
		if (cf_on_interval(weapon.rate, 0))
		{
			v2 pos = scene_node.get_global_transform().pos;
			v2 dir = {};

			bool has_target = false;
			if (weapon.target_type == TARGET_RANDOM_DIR)
			{
				has_target = true;
				dir = get_random_dir();
			}
			else if (weapon.target_type == TARGET_CLOSEST_ENEMY)
			{
				auto targeting_circle = cf_make_circle(
					pos,
					weapon.targeting_radius
				);
				has_target = try_get_closest_enemy_dir(targeting_circle, &dir);
			}

			if (!has_target)
			{
				return;
			}

			Entity e = ECS_NULL;
			if (weapon.weapon_type == WEAPON_BOOMERANG)
			{
				e = make_projectile_boomerang(world, pos, dir);
			}

			if (e == ECS_NULL)
			{
				return;
			}
		}
	});
}
