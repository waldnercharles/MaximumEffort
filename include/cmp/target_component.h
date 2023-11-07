#pragma once
#include <cute.h>
#include <entt/entt.hpp>

struct TargetComponent
{
	entt::entity entity = entt::null;

	bool face_target = false;
};
