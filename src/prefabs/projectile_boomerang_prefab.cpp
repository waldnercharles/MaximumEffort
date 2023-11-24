#include "prefabs/projectile_boomerang_prefab.h"
#include "cmp/damage_component.h"
#include "cmp/hit_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/lifetime_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_component.h"
#include "cmp/projectile_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

Entity prefabs::ProjectileBoomerang::create(World &w, v2 pos, v2 dir, u16 id)
{
	const Entity e = w.create();
	auto &proj = w.emplace<ProjectileComponent>(e);
	proj.piercing = true;

	auto &t = w.emplace<TransformComponent>(e);
	t.set_pos(pos);

	auto &behavior = w.emplace<MovementBehavior_ConstantDirectionComponent>(e);
	behavior.speed = {80, 80};
	behavior.dir = dir;

	auto &m = w.emplace<MovementComponent>(e);
	m.angular_vel = PI * 3.f;

	auto &hitbox = w.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = w.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 4);

	auto &damage = w.emplace<DamageComponent>(e);
	damage.min = 4;
	damage.max = 17;

	auto &hit = w.emplace<HitComponent>(e);
	hit.id = id;
	hit.frame_immunity = 255;

	auto &lifetime = w.emplace<LifetimeComponent>(e);
	lifetime = 2.0f;

	auto &s = w.emplace<SpriteComponent>(e, cf_make_sprite("boomerang.ase"));

	s.scale = cf_v2(0.85f, 0.85f);

	return e;
}
