#pragma once
#include <entt/entt.hpp>

enum EnemyType
{
	ENEMY_EYEBALL,
};

struct SpawnerComponent
{
	float rate;
	EnemyType entity_type;
};
