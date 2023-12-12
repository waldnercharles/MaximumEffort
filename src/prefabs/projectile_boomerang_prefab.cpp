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
	t.set_rotation(atan2(dir.y, dir.x));

	auto &behavior = w.emplace<RayMovementComponent>(e);
	behavior.speed = 540;
	behavior.dir = dir;
	behavior.damping = 0.5f;

	w.emplace<MovementComponent>(e);

	auto &hitbox = w.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = w.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 4);

	auto &damage = w.emplace<DamageComponent>(e);
	damage.min = 80;
	damage.max = 140;

	auto &hit = w.emplace<HitComponent>(e);
	hit.id = id;
	hit.frame_immunity = 255;

	auto &lifetime = w.emplace<LifetimeComponent>(e);
	lifetime = 2.0f;

	auto &s = w.emplace<SpriteComponent>(e, cf_make_sprite("arrow.ase"));

	s.scale = cf_v2(0.85f, 0.85f);

	return e;
}
