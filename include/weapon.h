#pragma once
#include "projectile.h"
#include <cute.h>

struct Weapon
{
	Projectile projectile_template;

	float timer;
	float rate;

	void update(float dt);

	Projectile spawn_projectile();
};
