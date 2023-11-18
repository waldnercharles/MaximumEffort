#pragma once
#include "lifetime.h"
#include <cute.h>

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

struct Weapon
{
	float rate = {};
	float targeting_radius = {};

	TargetType target_type = {};
	WeaponType weapon_type = {};
};
