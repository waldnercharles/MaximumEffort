#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct TargetComponent
{
	entt::entity entity = entt::null;
	Cute::v2 dir = {};

	bool face_target = false;
};
