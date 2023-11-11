#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "cute_aabb_grid.h"
#include "tiled_map.h"

#include <entt/entt.hpp>

struct Game
{
	Cute::Rnd rnd;
	Cute::v2 camera_size;
	Cute::v2 world_size;

	float spawn_radius;

	TiledMap map;

	entt::registry reg;

	Cute::AabbGrid enemy_grid;

	f64 total_time;
	bool paused;

	void update(float dt);
	void draw();
};

extern Game game;

void make_game();
