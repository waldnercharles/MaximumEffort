#pragma once
#include "lifetime_component.h"
#include <cute.h>
#include <entt/entt.hpp>

enum WeaponType
{
	WEAPON_BOOMERANG
};

struct WeaponComponent
{
	float rate = {};
	WeaponType weapon_type = {};
	entt::entity parent = entt::null;
};
