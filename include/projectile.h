#pragma once

#include <cute.h>

struct Projectile
{
	bool alive;

	float lifetime;

	Cute::v2 pos;
	Cute::v2 vel;

	Cute::v2 dir;
	Cute::v2 speed;

	Cute::Sprite sprite;

	void update(float dt);
	void draw();
};
