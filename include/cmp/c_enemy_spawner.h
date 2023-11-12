#pragma once

enum EnemyType
{
	ENEMY_EYEBALL,
};

struct C_EnemySpawner
{
	float rate;
	EnemyType entity_type;
};
