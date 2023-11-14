#pragma once
#include <cute.h>
#include <flecs.h>

struct C_Behavior_FollowTarget
{
	flecs::entity target = flecs::entity::null();
	Cute::v2 dir = {};
	Cute::v2 speed = {};

	bool face_target = false;
};
