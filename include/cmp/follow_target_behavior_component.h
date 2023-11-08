#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct FollowTargetBehaviorComponent
{
	entt::entity entity = entt::null;
	Cute::v2 dir = {};
	Cute::v2 speed = {};

	bool face_target = false;
};
