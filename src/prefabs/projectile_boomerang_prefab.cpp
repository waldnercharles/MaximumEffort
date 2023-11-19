#include "prefabs/projectile_boomerang_prefab.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/lifetime_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_component.h"
#include "cmp/projectile_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

Entity prefabs::ProjectileBoomerang::create(World &world, v2 pos, v2 dir)
{
	const Entity e = world.create();
	world.emplace<ProjectileComponent>(e);

	auto &t = world.emplace<TransformComponent>(e);
	t.set_pos(pos);

	auto &behavior = world.emplace<MovementBehavior_ConstantDirectionComponent>(
		e
	);
	behavior.speed = {80, 80};
	behavior.dir = dir;

	auto &m = world.emplace<MovementComponent>(e);
	m.angular_vel = PI * 3.f;

	auto &hitbox = world.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = world.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 24);

	auto &lifetime = world.emplace<LifetimeComponent>(e);
	lifetime = 2.0f;

	world.emplace<SpriteComponent>(e, cf_make_sprite("boomerang.ase"));

	return e;
}
