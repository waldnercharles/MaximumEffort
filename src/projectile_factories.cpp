#include "projectile_factories.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_lifetime.h"
#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_constant_direction.h"
#include "cmp/c_physics.h"
#include "cmp/c_projectile.h"
#include "cmp/c_sprite.h"
#include "cmp/c_transform.h"
#include "cute_math.h"

using namespace Cute;

entt::entity make_projectile_boomerang(entt::registry &reg, v2 pos, v2 dir)
{
	const entt::entity e = reg.create();
	reg.emplace<C_Projectile>(e);

	auto &scene_node = reg.emplace<C_Transform>(e);
	scene_node.set_pos(pos);

	auto &behavior = reg.emplace<C_MovementBehavior_ConstantDirection>(e);
	behavior.speed = V2(80, 80);
	behavior.dir = dir;

	auto &m = reg.emplace<C_Movement>(e);
	m.angular_vel = CF_PI * 3.f;// Rotations per second

	reg.emplace<C_Physics>(e, make_aabb(V2(0, 0), 17, 18));

	auto &hitbox = reg.emplace<C_Hitbox>(e);
	hitbox.circle = make_circle(V2(0, 0), 4);

	auto &hurtbox = reg.emplace<C_Hurtbox>(e);
	hurtbox.circle = make_circle(V2(0, 0), 24);

	auto &lifetime = reg.emplace<C_Lifetime>(e);
	lifetime = 2.0f;

	reg.emplace<C_Sprite>(e, make_sprite("boomerang.ase"));

	return e;
}