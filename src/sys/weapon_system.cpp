#include "sys/weapon_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_constant_direction.h"
#include "cmp/c_transform.h"
#include "cmp/c_weapon.h"
#include "game.h"
#include "projectile_factories.h"

#include <cute.h>

using namespace Cute;

v2 get_random_dir()
{
	float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
	return V2(cosf(angle), sinf(angle));
}

struct TargetClosestEnemyQueryData
{
	v2 player_pos = {};
	float radius = {};

	entt::entity entity = entt::null;

	float dist = {};
	v2 dir = {};
};

bool on_closest_enemy_query_hit(
	CF_Leaf leaf,
	CF_Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	entt::entity enemy = (entt::entity)(u64)leaf_udata;
	auto *query_data = (TargetClosestEnemyQueryData *)fn_udata;

	auto player_pos = query_data->player_pos;
	auto enemy_pos = reg.get<C_Transform>(enemy).get_global_transform().pos;

	float dist = distance(player_pos, enemy_pos);

	if (dist > query_data->radius)
	{
		return true;
	}

	if (query_data->entity == entt::null || dist < query_data->dist)
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
	data.entity = entt::null;
	data.player_pos = circle.p;
	data.radius = circle.r;

	aabb_tree_query(
		game.enemy_aabb_tree,
		on_closest_enemy_query_hit,
		make_aabb(circle.p, circle.r * 2, circle.r * 2),
		&data
	);

	*dir = data.dir;

	return data.entity != entt::null;
}

void weapon_system(entt::registry &reg, float dt)
{
	reg.view<C_Weapon, C_Transform>().each([&](auto weapon_entity,
											   C_Weapon &weapon,
											   C_Transform &scene_node) {
		if (on_interval(weapon.rate))
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
				auto targeting_circle = make_circle(
					pos,
					weapon.targeting_radius
				);
				has_target = try_get_closest_enemy_dir(targeting_circle, &dir);
			}

			if (!has_target)
			{
				return;
			}

			entt::entity e = entt::null;
			if (weapon.weapon_type == WEAPON_BOOMERANG)
			{
				e = make_projectile_boomerang(reg, pos, dir);
			}

			if (e == entt::null)
			{
				return;
			}
		}
	});
}
