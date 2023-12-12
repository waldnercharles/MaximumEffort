#pragma once
#include "enemy_type.h"

struct EnemySpawnerComponent
{
	const char *enemy_type;

	int start, end;

	float interval;
	int spawns_per_interval;
	int max_spawns;

	bool spawn_once;

	Array<Entity> spawned_enemies;
};
