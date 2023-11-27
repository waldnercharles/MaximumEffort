//#pragma once
//#include "cmp/player_component.h"
//#include "cmp/transform_component.h"
//#include "common.h"
//#include "enemy_type.h"
//#include "prefabs/enemy_eyeball_prefab.h"
//
//struct EnemySpawner
//{
//	EnemyType type;
//	int start;
//	int end;
//	float interval;
//	int spawns_per_interval;
//	int max_spawns;
//
//	int alive_count;
//};
//
//struct EnemySpawnerSystem
//{
//	const int MAX_SPAWNS_PER_FRAME = 5;
//	const float DEFAULT_TIME_REMAINING = 1800.f;
//
//	EnemySpawnerSystem(float spawn_radius);
//	void update(World &world);
//
//private:
//	v2 get_spawn_offset();
//
//	float time_remaining = DEFAULT_TIME_REMAINING;
//	f32 spawn_radius;
//
//	Array<EnemySpawner> spawners = {};
//	Rnd rnd = cf_rnd_seed(time(nullptr));
//};