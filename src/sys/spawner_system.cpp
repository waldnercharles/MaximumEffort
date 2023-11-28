#include "sys/spawner_system.h"
#include "cmp/enemy_component.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"
#include "game_timer.h"
#include "prefabs/enemy_prefab.h"
#include "sys/difficulty_system.h"

#include <cute.h>

SpawnerSystem::SpawnerSystem(
	f32 spawn_radius,
	f32 respawn_radius,
	const GameTimer &game_timer,
	const DifficultySystem &difficulty_system
)
	: rnd(cf_rnd_seed((u64)time(nullptr))),
	  spawn_radius(spawn_radius),
	  respawn_radius_sq(respawn_radius * respawn_radius),
	  game_timer(game_timer),
	  difficulty_system(difficulty_system)
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

			if (cf_on_interval(s.interval, 0))
			{
				for (int i = 0; i < s.spawns_per_interval; i++)
				{
					if (s.spawned_enemies.count() >= s.max_spawns)
					{
						break;
					}

					Entity enemy;
					auto modifier = difficulty_system.get_stats_modifier();
					const char *name = "blue_slime.ase";
					switch (s.enemy_type)
					{
						case ENEMY_TYPE_BLUE_SLIME:
							name = "blue_slime.ase";
							modifier.health.flat += 400;
							break;
						case ENEMY_TYPE_GREEN_SLIME:
							name = "green_slime.ase";
							modifier.health.flat += 50;
							modifier.speed.flat += 8;
							break;
						case ENEMY_TYPE_GREY_SLIME:
							name = "grey_slime.ase";
							modifier.health.flat += 250;
							modifier.speed.flat -= 5;
							break;
						case ENEMY_TYPE_ORANGE_SLIME:
							name = "orange_slime.ase";
							modifier.health.flat += 750;
							modifier.speed.flat += 3;
							break;
						case ENEMY_TYPE_RED_SLIME:
							name = "red_slime.ase";
							modifier.health.flat += 10000;
							break;
						case ENEMY_TYPE_EYEBALL:
							name = "eyeball.ase";
							break;
					}
					enemy = prefabs::Enemy::create(
						world,
						player_pos + get_spawn_offset(),
						modifier,
						name
					);

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
