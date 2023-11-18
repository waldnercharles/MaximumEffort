#pragma once

enum EnemyType
{
	ENEMY_EYEBALL,
};

struct EnemySpawner
{
	float rate;
	EnemyType entity_type;
};
