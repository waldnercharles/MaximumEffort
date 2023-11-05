#include "enemy.h"
#include "game.h"
#include "imgui.h"

using namespace Cute;

void Enemy::update(float dt)
{
	// Limit max distance enemy may be from the player
	Aabb bounds = get_max_enemy_bounds();
	if (!cf_contains_point(bounds, pos))
	{
		pos = get_enemy_spawn_pos();
	}

	// Follow the player
	v2 dir = safe_norm(game.player.pos - pos);
	v2 vel = dir * speed;

	pos = pos + vel * dt;

	sprite.transform.p = pos;
	sprite.transform.r.s = dir.y;
	sprite.transform.r.c = dir.x;
}

void Enemy::draw()
{
	sprite.update();
	sprite.draw();
}
