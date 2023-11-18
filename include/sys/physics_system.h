#pragma once
#include "common.h"
#include "geometry.h"
#include "spatial_hash.h"
#include <entt/entt.hpp>

struct PhysicsSystem
{
	PhysicsSystem();

	void update(f32 delta_time);

private:
	AabbGrid<Entity> grid;

	void update_spatial_hash(World &reg);
	void handle_enemy_to_enemy_collisions(World &reg);
};

//	void physics_system(World &reg, float dt);
