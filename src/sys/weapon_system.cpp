#include "sys/weapon_system.h"
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
	Cute::AabbGridNode,
	Cute::Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	entt::entity enemy = (entt::entity)(uint64_t)leaf_udata;
	if (!reg.valid(enemy))
	{
		return true;
	}

	auto *query_data = (TargetClosestEnemyQueryData *)fn_udata;

	auto player_pos = query_data->player_pos;
	auto enemy_pos = reg.get<C_WorldTransform>(enemy).pos;

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

	auto aabb = make_aabb(circle.p, circle.r * 2, circle.r * 2);
	aabb_grid_query(game.enemy_grid, aabb, on_closest_enemy_query_hit, &data);

	*dir = data.dir;

	return data.entity != entt::null;
}

void add_weapon_system(flecs::world *world)
{
	world->system<C_Weapon, C_WorldTransform>("weapon").each(
		[](flecs::iter &it, size_t, C_Weapon &w, C_WorldTransform &t) {
			flecs::world world = it.world();
			if (on_interval(w.rate))
			{
				v2 pos = t.pos;
				v2 dir = {};

				bool has_target = false;
				if (w.target_type == TARGET_RANDOM_DIR)
				{
					has_target = true;
					dir = get_random_dir();
				}
				else if (w.target_type == TARGET_CLOSEST_ENEMY)
				{
					Circle c = make_circle(pos, w.targeting_radius);
					has_target = try_get_closest_enemy_dir(c, &dir);
				}

				if (!has_target)
				{
					return;
				}

				flecs::entity e = flecs::entity::null();
				if (w.weapon_type == WEAPON_BOOMERANG)
				{
					e = make_projectile_boomerang(&world, pos, dir);
				}

				if (e == flecs::entity::null())
				{
					return;
				}
			}
		}
	);
}
