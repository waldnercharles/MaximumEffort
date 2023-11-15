#pragma once
#include "cute_aabb_grid.h"

#include <flecs.h>

struct Region
{
};

struct EcsAabbGrid
{
	int w = 0;
	int h = 0;

	Cute::v2 pos = {};

	int region_size = 0;
	Cute::Array<flecs::entity> map = {};
};

void add_physics_system();
