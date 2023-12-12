#include "sys/spawner_system.h"
#include "cmp/enemy_component.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/health_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"
#include "game.h"
#include "game_timer.h"
#include "log.h"
#include "prefabs/enemy_prefab.h"
#include "sys/difficulty_system.h"

#include <cute.h>

SpawnerSystem::SpawnerSystem(
	f32 spawn_radius,
	f32 respawn_radius,
	const GameTimer &game_timer,
	const DifficultySystem &difficulty_system,
	const EnemeyPrototypeMap &prototypes
)
	: rnd(cf_rnd_seed((u64)time(nullptr))),
	  spawn_radius(spawn_radius),
	  respawn_radius_sq(respawn_radius * respawn_radius),
	  game_timer(game_timer),
	  difficulty_system(difficulty_system),
	  prototypes(prototypes)
{
}

void SpawnerSystem::update(World &world)
{
	auto player = world.view<PlayerComponent, TransformComponent>().front();
	auto &player_transform = world.get<TransformComponent>(player);

	v2 player_pos = player_transform.get_world_transform().pos;

	// Respawn if out of bounds
	auto enemies = world.view<EnemyComponent, TransformComponent>();
	for (auto e : enemies)
	{
		auto &enemy_scene_node = enemies.get<TransformComponent>(e);
		auto enemy_pos = enemy_scene_node.get_world_transform().pos;

		if (cf_len_sq(enemy_pos - player_pos) > respawn_radius_sq)
		{
			enemy_scene_node.set_pos(player_pos + get_spawn_offset());
		}
	}

	// Actually spawn new enemies
	world.view<EnemySpawnerComponent>().each([&](auto e,
												 EnemySpawnerComponent &s) {
		float time_remaining = game_timer.get_time_remaining();
		if (s.start > time_remaining)
		{
			if (s.end > time_remaining)
			{
				world.destroy(e);
				return;
			}

			// Update alive count
			for (int i = s.spawned_enemies.count() - 1; i >= 0; i--)
			{
				// TODO: Should we use an event here instead?
				if (!world.valid(s.spawned_enemies[i]))
				{
					s.spawned_enemies.unordered_remove(i);
				}
			}

			if (game_timer.on_interval(s.interval))
			{
				for (int i = 0; i < s.spawns_per_interval; i++)
				{
					if (s.spawned_enemies.count() >= s.max_spawns)
					{
						break;
					}

					auto pos = player_pos + get_spawn_offset();

					auto *spawn_fn = prototypes.try_get(sintern(s.enemy_type));
					if (!spawn_fn)
					{
						log_error("Unable to find enemy: {}", s.enemy_type);
						return;
					}

					auto enemy = (*spawn_fn)(world, rnd, pos);
					auto &stats = world.get<StatsComponent>(enemy);
					auto &health = world.get<HealthComponent>(enemy);
					health.current = stats.get_stats().health;

					s.spawned_enemies.add(enemy);
				}

				if (s.spawn_once)
				{
					world.destroy(e);
				}
			}
		}
	});
}

inline v2 SpawnerSystem::get_spawn_offset()
{
	float angle = cf_rnd_next_range_float(&rnd, -PI, PI);
	v2 dir = cf_v2(cosf(angle), sinf(angle));

	return (dir * spawn_radius);
}
