#include "sys/spawner_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_enemy_spawner.h"
#include "cmp/c_player.h"
#include "cmp/c_transform.h"
#include "factories.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

Cute::v2 get_enemy_spawn_pos(v2 center_pos)
{
	float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
	v2 dir = V2(cosf(angle), sinf(angle));

	return center_pos + (dir * game.spawn_radius);
}

void spawner_system(entt::registry &reg, float dt)
{
	auto player = reg.view<C_Player, C_Transform>().front();
	auto &player_scene_node = reg.get<C_Transform>(player);

	v2 player_pos = player_scene_node.get_global_transform().pos;
	v2 world_size = game.world_size;
	Aabb bounds = make_aabb(player_pos, world_size.x, world_size.y);

	// Respawn if out of bounds
	auto enemies = reg.view<C_Enemy, C_Transform>();
	for (auto e : enemies)
	{
		auto &enemy_scene_node = enemies.get<C_Transform>(e);
		auto enemy_pos = enemy_scene_node.get_global_transform().pos;

		if (!contains(bounds, enemy_pos))
		{
			enemy_scene_node.set_pos(get_enemy_spawn_pos(player_pos));
		}
	}

	// Actually spawn new enemies
	auto spawners = reg.view<C_EnemySpawner>();
	for (auto e : spawners)
	{
		auto &s = spawners.get<C_EnemySpawner>(e);

		if (on_interval(s.rate))
		{
			switch (s.entity_type)
			{
				case ENEMY_EYEBALL:
				{
					make_enemy_eyeball(
						game.reg,
						get_enemy_spawn_pos(player_pos),
						player
					);
					break;
				}
			}
		}
	}
}
