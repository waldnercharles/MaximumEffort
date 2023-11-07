#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "tiled_map.h"

#include <entt/entt.hpp>

struct Game
{
	Cute::Rnd rnd;
	Cute::v2 camera_size;

	TiledMap map;

	entt::registry reg;

	Cute::AabbTree enemy_aabb_tree;

	f64 total_time;
	bool paused;

	void update(float dt);
	void draw();
};

extern Game game;

void make_game();