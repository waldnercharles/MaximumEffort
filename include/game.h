#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "sys/physics_system.h"
#include "tiled_map.h"

#include <entt/entt.hpp>

struct Game
{
	Rnd rnd;
	v2 camera_size;

	float spawn_radius;
	v2 world_size;

	//	AabbGrid enemy_grid;

	TiledMap map;

	World world;

	f64 total_time;
	bool paused;

	PhysicsSystem physics_system;

	void update(float dt);
	void draw();
};

extern Game game;

void make_game();
void destroy_game(Game game);
