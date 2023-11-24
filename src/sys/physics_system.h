#pragma once
#include "aabb_grid.h"
#include "common.h"
#include "system.h"

struct PhysicsSystem final : public System
{
	PhysicsSystem(AabbGrid<Entity> &enemy_grid);

	void update(World &world) override;

private:
	void handle_enemy_to_enemy_collisions(World &w);

	AabbGrid<Entity> &enemy_grid;
};

//	void physics_system(World &reg, float dt);
