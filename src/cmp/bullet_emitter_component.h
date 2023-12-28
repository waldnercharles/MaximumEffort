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

	float projectiles_per_interval = 1;
	float interval = {};

	float emission_radius = {};
	float emission_degrees = {};
	v2 emission_offset = {};

	const char *bullet_type = nullptr;

	//	TargetType target_type = {};
	//	WeaponType weapon_type = {};
};
