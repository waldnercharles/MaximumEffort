#include "projectile_factories.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/lifetime_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_component.h"
#include "cmp/projectile_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "common.h"
#include "cute_math.h"

Entity make_projectile_boomerang(World &w, v2 pos, v2 dir)
{
	const Entity e = w.create();
	w.emplace<ProjectileComponent>(e);

	auto &t = w.emplace<TransformComponent>(e);
	t.set_pos(pos);

	auto &behavior = w.emplace<MovementBehavior_ConstantDirectionComponent>(e);
	behavior.speed = {80, 80};
	behavior.dir = dir;

	auto &m = w.emplace<MovementComponent>(e);
	m.angular_vel = PI * 3.f;// Rotations per second

	auto &hitbox = w.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = w.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 24);

	auto &lifetime = w.emplace<LifetimeComponent>(e);
	lifetime = 2.0f;

	w.emplace<SpriteComponent>(e, cf_make_sprite("boomerang.ase"));

	return e;
}