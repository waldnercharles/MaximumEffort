#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "cute_aabb_grid.h"
#include "sys/physics_system.h"
#include "tiled_map.h"

#include <entt/entt.hpp>

struct Game
{
	Cute::Rnd rnd;
	Cute::v2 camera_size;

	float spawn_radius;
	Cute::v2 world_size;

	Cute::AabbGrid enemy_grid;

	TiledMap map;

	entt::registry reg;

	f64 total_time;
	bool paused;

	PhysicsSystem physics_system;

	void update(float dt);
	void draw();
};

extern Game game;

Game make_game();
void destroy_game(Game game);
