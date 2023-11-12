#pragma once
#include <cute.h>

struct C_MovementBehavior_FollowTarget
{
	entt::entity entity = entt::null;
	Cute::v2 dir = {};
	Cute::v2 speed = {};

	bool face_target = false;
};
