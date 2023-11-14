#pragma once
#include "cute_aabb_grid.h"

#include <flecs.h>

void add_physics_system(flecs::world *world, Cute::AabbGrid &grid);

void physics_system(flecs::world &world);
