#pragma once
#include "lifetime_component.h"
#include <cute.h>
#include <entt/entt.hpp>

enum WeaponType
{
	WEAPON_BOOMERANG
};

enum TargetType
{
	TARGET_RANDOM_DIR,
	TARGET_RANDOM_ENEMY,
	TARGET_CLOSEST_ENEMY,
	TARGET_PLAYER
};

struct WeaponComponent
{
	float rate = {};
	float targeting_radius = {};

	TargetType target_type = {};
	WeaponType weapon_type = {};

	entt::entity parent = entt::null;
};
