#pragma once
#include <cute.h>

struct MovementBehavior_FollowTargetComponent
{
	Entity entity = entt::null;
	v2 dir = {};

	bool face_target = false;
};
