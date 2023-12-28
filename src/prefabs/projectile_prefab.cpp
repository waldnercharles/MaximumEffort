#include "prefabs/projectile_prefab.h"
#include "cmp/damage_component.h"
#include "cmp/hit_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/lifetime_component.h"
#include "cmp/movement_behavior_constant_direction_component.h"
#include "cmp/movement_component.h"
#include "cmp/projectile_component.h"
#include "cmp/sprite_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"

Entity
prefabs::Projectile::create(World &w, Entity owner, v2 pos, float angle, u16 id)
{
	auto &owner_stats = w.get<C_Stats>(owner);

	const Entity e = w.create();
	auto &proj = w.emplace<C_Projectile>(e);
	proj.piercing = false;

	auto &t = w.emplace<C_Transform>(e);
	t.set_pos(pos);
	t.set_rotation(angle);

	auto &behavior = w.emplace<C_RayMovement>(e);
	behavior.speed = 540;
	behavior.dir = cf_v2(cosf(angle), sinf(angle));
	behavior.damping = 0.5f;

	w.emplace<C_Movement>(e);

	auto &hitbox = w.emplace<C_Hitbox>(e);
	hitbox.circle = cf_make_circle({0, 0}, 4);

	auto &hurtbox = w.emplace<C_Hurtbox>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 4);

	auto &damage = w.emplace<C_Damage>(e);
	damage.min = owner_stats.get(Stat::damage);
	damage.max = owner_stats.get(Stat::damage);

	auto &hit = w.emplace<C_Hit>(e);
	hit.id = id;
	hit.frame_immunity = 255;

	auto &lifetime = w.emplace<C_Lifetime>(e);
	lifetime = 2.0f;

	auto &s = w.emplace<C_Sprite>(e, cf_make_sprite("arrow.ase"));

	s.scale = cf_v2(0.85f, 0.85f);

	return e;
}
