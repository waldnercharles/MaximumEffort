#pragma once
#include <cute.h>
#include <flecs.h>

struct C_MovementBehavior_FollowTarget
{
	flecs::entity entity = flecs::entity::null();
	Cute::v2 dir = {};
	Cute::v2 speed = {};

	bool face_target = false;
};
