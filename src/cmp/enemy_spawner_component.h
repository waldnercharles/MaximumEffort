#pragma once

struct C_EnemySpawner
{
	const char *enemy_type;

	int start, end;

	float interval;
	int spawns_per_interval;
	int max_spawns;

	bool spawn_once;

	Array<Entity> spawned_enemies;
};
