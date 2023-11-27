//#include "level.h"
//
//EnemySpawnerSystem::EnemySpawnerSystem(float spawn_radius)
//	: spawn_radius(spawn_radius)
//{
//}
//
//void EnemySpawnerSystem::update(World &world)
//{
//	time_remaining -= CF_DELTA_TIME_FIXED;
//
//	auto player = world.view<PlayerComponent>().front();
//	const auto player_pos =
//		world.get<TransformComponent>(player).get_world_transform().pos;
//
//	for (auto &s : spawners)
//	{
//		if (cf_on_interval(s.interval, 0))
//		{
//			for (int i = 0; i < s.spawns_per_interval; i++)
//			{
//				prefabs::EnemyEyeball::create(
//					world,
//					player_pos + get_spawn_offset(),
//					player
//				);
//			}
//		}
//	}
//}
//
//inline v2 EnemySpawnerSystem::get_spawn_offset()
//{
//	float angle = cf_rnd_next_range_float(&rnd, -PI, PI);
//	v2 dir = cf_v2(cosf(angle), sinf(angle));
//
//	return (dir * spawn_radius);
//}
