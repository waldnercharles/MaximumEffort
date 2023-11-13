#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct C_LocalTransform
{
	Cute::v2 pos;
	float angle;
};

struct C_WorldTransform
{
	Cute::v2 pos;
	float angle;
};