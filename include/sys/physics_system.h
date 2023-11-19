#pragma once
#include "aabb_grid.h"
#include "common.h"
#include "system.h"

struct PhysicsSystem : public System
{
	PhysicsSystem();

	void update(World &world) override;

private:
	AabbGrid<Entity> grid;

	void update_spatial_hash(World &reg);
	void handle_enemy_to_enemy_collisions(World &reg);
};

//	void physics_system(World &reg, float dt);
