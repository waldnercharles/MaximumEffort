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

flecs::entity make_projectile_boomerang(flecs::world *world, v2 pos, v2 dir)
{
	flecs::entity e = world->entity();
	e.add<C_Projectile>();
	e.set<C_WorldTransform>({pos, 0.0f});
	e.set<C_LocalTransform>({pos, 0.0f});
	e.set<C_Behavior_ConstantDirection>({dir, {80, 80}});
	e.set<C_Movement>({{}, CF_PI * 3.f});

	Aabb aabb = make_aabb({}, 17, 18);
	e.set<C_Physics>({aabb});

	e.set<C_Hitbox>({make_circle(center(aabb), 6)});
	e.set<C_Hurtbox>({make_circle(center(aabb), 8)});

	e.set<C_Lifetime>({2.0f});
	e.set<C_Sprite>({make_sprite("boomerang.ase"), 0});

	return e;
}