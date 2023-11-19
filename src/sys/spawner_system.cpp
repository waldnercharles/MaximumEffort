#include "sys/spawner_system.h"
#include "cmp/enemy_component.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"
#include "factories.h"

#include <cute.h>

SpawnerSystem::SpawnerSystem(f32 spawn_radius, f32 respawn_radius)
	: spawn_radius(spawn_radius),
	  respawn_radius(respawn_radius),
	  rnd(cf_rnd_seed((u64)time(nullptr)))
{
}

void SpawnerSystem::update(World &world)
{
	auto player = world.view<PlayerComponent, TransformComponent>().front();
	auto &player_scene_node = world.get<TransformComponent>(player);

	// TODO: Attach an enemy to its spawner.
	v2 spawner_pos = player_scene_node.get_world_transform().pos;
	Aabb bounds = cf_make_aabb_center_half_extents(
		spawner_pos,
		cf_v2(respawn_radius, respawn_radius)
	);

	// Respawn if out of bounds
	auto enemies = world.view<EnemyComponent, TransformComponent>();
	for (auto e : enemies)
	{
		auto &enemy_scene_node = enemies.get<TransformComponent>(e);
		auto enemy_pos = enemy_scene_node.get_world_transform().pos;

		if (!cf_contains_point(bounds, enemy_pos))
		{
			enemy_scene_node.set_pos(spawner_pos + get_spawn_pos());
		}
	}

	// Actually spawn new enemies
	auto spawners = world.view<EnemySpawnerComponent>();
	for (auto e : spawners)
	{
		auto &s = spawners.get<EnemySpawnerComponent>(e);

		if (cf_on_interval(s.rate, 0))
		{
			switch (s.entity_type)
			{
				case ENEMY_EYEBALL:
				{
					make_enemy_eyeball(
						world,
						spawner_pos + get_spawn_pos(),
						player
					);
					break;
				}
			}
		}
	}
}

inline v2 SpawnerSystem::get_spawn_pos()
{
	float angle = cf_rnd_next_range_float(&rnd, -PI, PI);
	v2 dir = cf_v2(cosf(angle), sinf(angle));

	return (dir * spawn_radius);
}
