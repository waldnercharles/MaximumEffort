#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include "player.h"
#include "projectile.h"
#include "tiled_map.h"
#include "weapon.h"

struct Game
{
	Cute::Rnd rnd;

	Player player;
	TiledMap map;

	Cute::Array<Weapon> weapons;
	Cute::Array<Projectile> projectiles;

	Cute::Array<Enemy> enemies;

	Cute::v2 camera_size;

	f64 total_time;

	bool paused;

	void update(float dt);
	void draw();

	Cute::Array<EnemySpawner> spawners;
};

extern Game game;

void make_game();