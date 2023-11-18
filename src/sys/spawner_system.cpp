#include "sys/spawner_system.h"
#include "cmp/enemy.h"
#include "cmp/enemy_spawner.h"
#include "cmp/player.h"
#include "cmp/transform.h"
#include "factories.h"
#include "game.h"

#include <cute.h>

v2 get_enemy_spawn_pos(v2 center_pos)
{
	float angle = cf_rnd_next_range_float(&game.rnd, -PI, PI);
	v2 dir = cf_v2(cosf(angle), sinf(angle));

	return center_pos + (dir * game.spawn_radius);
}

void spawner_system(World &w, float dt)
{
	auto player = w.view<Player, Transform>().front();
	auto &player_scene_node = w.get<Transform>(player);

	v2 player_pos = player_scene_node.get_global_transform().pos;
	v2 world_size = game.world_size;
	Aabb bounds = cf_make_aabb_pos_w_h(player_pos, world_size.x, world_size.y);

	// Respawn if out of bounds
	auto enemies = w.view<Enemy, Transform>();
	for (auto e : enemies)
	{
		auto &enemy_scene_node = enemies.get<Transform>(e);
		auto enemy_pos = enemy_scene_node.get_global_transform().pos;

		if (!cf_contains_point(bounds, enemy_pos))
		{
			enemy_scene_node.set_pos(get_enemy_spawn_pos(player_pos));
		}
	}

	// Actually spawn new enemies
	auto spawners = w.view<EnemySpawner>();
	for (auto e : spawners)
	{
		auto &s = spawners.get<EnemySpawner>(e);

		if (cf_on_interval(s.rate, 0))
		{
			switch (s.entity_type)
			{
				case ENEMY_EYEBALL:
				{
					make_enemy_eyeball(
						game.world,
						get_enemy_spawn_pos(player_pos),
						player
					);
					break;
				}
			}
		}
	}
}
