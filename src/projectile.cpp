#include "projectile.h"

using namespace Cute;

void Projectile::update(float dt)
{
	if (lifetime > dt)
	{
		lifetime -= dt;

		vel = dir * speed * dt;
		pos = pos + vel;

		sprite.transform.p = pos;

		sprite.transform.r = cf_sincos_f(CF_PI * 4.f * lifetime);
	}
	else
	{
		alive = false;
	}
}

void Projectile::draw()
{
	sprite.update();
	sprite.draw();
}
