#include "weapon.h"
#include "game.h"

using namespace Cute;

Projectile Weapon::spawn_projectile()
{
	v2 pos = game.player.pos;

	float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
	v2 dir = V2(cosf(angle), sinf(angle));

	Projectile projectile = projectile_template;
	projectile.alive = true;
	projectile.pos = pos;
	projectile.dir = dir;

	return projectile;
}

void Weapon::update(float dt)
{
	timer += dt;
	while (timer > rate)
	{
		timer -= rate;
		game.projectiles.add(spawn_projectile());
	}
}
