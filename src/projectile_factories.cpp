#include "projectile_factories.h"
#include "cmp/hitbox.h"
#include "cmp/hurtbox.h"
#include "cmp/lifetime.h"
#include "cmp/movement.h"
#include "cmp/movement_behavior_constant_direction.h"
#include "cmp/projectile.h"
#include "cmp/sprite.h"
#include "cmp/transform.h"
#include "common.h"
#include "cute_math.h"

Entity make_projectile_boomerang(World &w, v2 pos, v2 dir)
{
	const Entity e = w.create();
	w.emplace<Projectile>(e);

	auto &t = w.emplace<Transform>(e);
	t.set_pos(pos);

	auto &behavior = w.emplace<MovementBehavior_ConstantDirection>(e);
	behavior.speed = {80, 80};
	behavior.dir = dir;

	auto &m = w.emplace<Movement>(e);
	m.angular_vel = PI * 3.f;// Rotations per second

	auto &hitbox = w.emplace<Hitbox>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = w.emplace<Hurtbox>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 24);

	auto &lifetime = w.emplace<Lifetime>(e);
	lifetime = 2.0f;

	w.emplace<Sprite>(e, cf_make_sprite("boomerang.ase"));

	return e;
}