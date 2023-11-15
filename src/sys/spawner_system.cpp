#include "sys/spawner_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_enemy_spawner.h"
#include "cmp/c_transform.h"
#include "factories.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

v2 get_enemy_spawn_pos(v2 center_pos)
{
	float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
	v2 dir = V2(cosf(angle), sinf(angle));

	return center_pos + (dir * game.spawn_radius);
}

void add_spawner_system(flecs::world *world)
{
	//	world
	//		->system<const C_Enemy, const C_WorldTransform, C_LocalTransform>(
	//			"enemy_respawn"
	//		)
	//		.iter([](flecs::iter &it,
	//				 const C_Enemy *,
	//				 const C_WorldTransform *w,
	//				 C_LocalTransform *l) {
	//			const auto player = it.world().lookup("player");
	//			const C_WorldTransform *p_world = player.get<C_WorldTransform>();
	//			const auto world_size = game.world_size;
	//			Aabb bounds = make_aabb(p_world->pos, world_size.x, world_size.y);
	//
	//			for (auto i : it)
	//			{
	//				auto &e_world = w[i];
	//				auto &e_local = l[i];
	//				if (!contains(bounds, e_world.pos))
	//				{
	//					e_local.pos = get_enemy_spawn_pos(p_world->pos);
	//				}
	//			}
	//		});

	// // Actually spawn new enemies
	world->system<const C_EnemySpawner>("enemy_spawner")
		.iter([](flecs::iter &it, const C_EnemySpawner *s) {
			flecs::world world = it.world();

			const auto player = world.lookup("player");
			const C_WorldTransform *p_world = player.get<C_WorldTransform>();

			for (auto i : it)
			{
				auto &spawner = s[i];
				if (on_interval(spawner.rate))
				{
					if (on_interval(spawner.rate))
					{
						switch (spawner.entity_type)
						{
							case ENEMY_EYEBALL:
							{
								make_enemy_eyeball(
									&world,
									get_enemy_spawn_pos(p_world->pos)
								);
								break;
							}
						}
					}
				}
			}
		});
}
