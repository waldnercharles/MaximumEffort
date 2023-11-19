#pragma once

enum EnemyType
{
	ENEMY_EYEBALL,
};

struct EnemySpawnerComponent
{
	float rate;
	EnemyType entity_type;
};
