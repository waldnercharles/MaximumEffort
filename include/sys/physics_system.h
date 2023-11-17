#pragma once
#include "common.h"
#include "ecs.h"
#include "geometry.h"
#include "spatial_hash.h"
#include <entt/entt.hpp>

struct PhysicsSystem
{
	PhysicsSystem();

	void update(f32 delta_time);

private:
	AabbGrid<entt::entity> grid;

	void update_spatial_hash(entt::registry &reg);
	void handle_enemy_to_enemy_collisions(entt::registry &reg);
};

//	void physics_system(entt::registry &reg, float dt);
