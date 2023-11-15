#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "cute_aabb_grid.h"
#include "sys/physics_system.h"
#include "tiled_map.h"

#include <entt/entt.hpp>
#include <flecs.h>

struct Game
{
	Cute::Rnd rnd;
	Cute::v2 camera_size;

	float spawn_radius;
	Cute::v2 world_size;

	EcsAabbGrid enemy_grid;

	TiledMap map;

	flecs::world *world;
	entt::registry reg;

	f64 total_time;
	bool paused;

	void update(float dt);
	void draw();
};

extern Game game;

void make_game();
