#pragma once
#include "common.h"
#include "prefabs/enemy_spawner_prefab.h"

void make_level_1(World &world, Entity player)
{
	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_GREEN_SLIME,
		1800,
		1690,
		0.4f,
		2,
		25,
		false,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_ORANGE_SLIME,
		1740,
		1700,
		0.1f,
		5,
		5,
		true,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_GREY_SLIME,
		1680,
		1570,
		0.25f,
		2,
		40,
		false,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_RED_SLIME,
		1660,
		1580,
		0.1f,
		1,
		1,
		true,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_GREEN_SLIME,
		1640,
		1585,
		20,
		5,
		10,
		false,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_GREEN_SLIME,
		1560,
		1450,
		2.5,
		50,
		100,
		false,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_BLUE_SLIME,
		1520,
		1500,
		0.1,
		15,
		15,
		true,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_BLUE_SLIME,
		1480,
		1460,
		0.1,
		25,
		25,
		true,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_ORANGE_SLIME,
		1440,
		1330,
		0.2f,
		1,
		25,
		false,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_RED_SLIME,
		1420,
		1365,
		0.1f,
		1,
		1,
		true,
		player
	);

	prefabs::EnemySpawner::create(
		world,
		ENEMY_TYPE_BLUE_SLIME,
		1420,
		1365,
		2.f,
		5,
		10,
		false,
		player
	);
}