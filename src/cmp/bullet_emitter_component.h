#pragma once
#include "lifetime_component.h"
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

struct C_BulletEmitter
{
	Entity owner;

	float rate = {};
	float targeting_radius = {};

	TargetType target_type = {};
	WeaponType weapon_type = {};
};
