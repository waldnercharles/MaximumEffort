#pragma once
#include <cute.h>

struct MovementBehavior_FollowTarget
{
	Entity entity = entt::null;
	v2 dir = {};
	v2 speed = {};

	bool face_target = false;
};
