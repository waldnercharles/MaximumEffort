#include "enemy_spawner.h"

#include "enemy.h"
#include "game.h"

#include <cute.h>
#include <cute_math.h>

using namespace Cute;

Enemy spawn_enemy(EnemyType enemy_type)
{
	Enemy enemy = {};

	enemy.pos = get_enemy_spawn_pos();

	switch (enemy_type)
	{
		case ENEMY_TYPE_EYEBALL:
		{
			enemy.speed = V2(20, 20);
			enemy.sprite = make_sprite("eyeball.ase");
			break;
		}
	}

	return enemy;
}

void EnemySpawner::update(float dt)
{
	timer += dt;

	while (timer > rate)
	{
		timer -= rate;
		game.enemies.add(spawn_enemy(enemy_type));
	}
}

Aabb get_max_enemy_bounds()
{
	return make_aabb_center_half_extents(
		game.player.pos,
		game.camera_size * 0.55f
	);
}

Cute::v2 get_enemy_spawn_pos()
{
	// TODO: This is good enough for now, but could be better.
	float angle = rnd_next_range(game.rnd, -CF_PI * 2.0f, CF_PI * 2.0f);

	v2 dir = V2(cosf(angle), sinf(angle));

	Aabb bounds = get_max_enemy_bounds();
	return clamp(bounds, center(bounds) + dir * cf_len(half_extents(bounds)));
}
