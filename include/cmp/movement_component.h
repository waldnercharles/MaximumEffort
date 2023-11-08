#pragma once

#include <cute.h>

struct MovementComponent
{
	Cute::v2 vel = {};
	float angular_vel = {};
};