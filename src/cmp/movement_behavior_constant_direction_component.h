#pragma once
#include "common.h"
#include <cute.h>

struct RayMovementComponent
{
	v2 dir;

	float speed;
	float acceleration;
	float damping;
};

struct MovementBehavior_ConstantDirectionComponent
{
	v2 dir;
	v2 speed;
};