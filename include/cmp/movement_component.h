#pragma once

#include <cute.h>

struct MovementComponent
{
	Cute::v2 pos = {};
	Cute::v2 vel = {};

	float angle = {};
	float angular_vel = {};

	Cute::v2 dir = {};
	Cute::v2 speed = {};
};