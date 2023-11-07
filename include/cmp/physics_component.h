#pragma once
#include <cute.h>

struct PhysicsComponent
{
	Cute::Aabb aabb = {};
	Cute::Leaf leaf = {-1};
};
