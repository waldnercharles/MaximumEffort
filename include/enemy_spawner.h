#pragma once

#include "enemy_type.h"
#include <cute.h>

struct Game;

struct EnemySpawner
{
	float timer;
	float rate;

	EnemyType enemy_type;

	void update(float dt);
};

Cute::Aabb get_max_enemy_bounds();
Cute::v2 get_enemy_spawn_pos();
