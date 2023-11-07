#include "projectile_factories.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/lifetime_component.h"
#include "cmp/movement_component.h"
#include "cmp/physics_component.h"
#include "cmp/projectile_component.h"
#include "cmp/sprite_component.h"

using namespace Cute;

entt::entity make_projectile_boomerang(entt::registry &reg, v2 pos, v2 dir)
{
	const entt::entity e = reg.create();
	reg.emplace<ProjectileComponent>(e);

	auto &m = reg.emplace<MovementComponent>(e);
	m.pos = pos;
	m.dir = dir;
	m.speed = V2(80, 80);
	m.angular_vel = CF_PI * 3.f;// Rotations per second

	reg.emplace<PhysicsComponent>(e, make_aabb(V2(0, 0), 17, 18));

	auto &hitbox = reg.emplace<HitboxComponent>(e);
	hitbox.circle = make_circle(V2(8, 8), 8);

	auto &hurtbox = reg.emplace<HurtboxComponent>(e);
	hurtbox.circle = make_circle(V2(8, 8), 8);

	auto &lifetime = reg.emplace<LifetimeComponent>(e);
	lifetime = 2.0f;

	reg.emplace<SpriteComponent>(e, make_sprite("boomerang.ase"));

	return e;
}