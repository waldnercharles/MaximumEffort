#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include "player.h"
#include "tiled_map.h"

struct Game
{
	Cute::Rnd rnd;

	Player player;
	TiledMap map;

	Cute::Array<Enemy> enemies;

	Cute::v2 camera_size;

	f64 total_time;
	f64 spawn_timer;

	bool paused;

	void update(float dt);
	void draw();

	Cute::Array<EnemySpawner> spawners;
};

extern Game game;

void make_game();