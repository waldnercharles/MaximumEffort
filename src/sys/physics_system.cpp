#include "sys/physics_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_physics.h"
#include "cmp/c_player.h"
#include "cmp/c_projectile.h"
#include "cmp/c_transform.h"
#include "cute_aabb_grid.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

bool on_hurtbox_to_hitbox(
	Cute::AabbGridNode id,
	Cute::Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	entt::entity hurtbox_entity = *(entt::entity *)fn_udata;
	if (!reg.valid(hurtbox_entity))
	{
		return false;
	}

	entt::entity hitbox_entity = (entt::entity)(uint64_t)leaf_udata;
	if (!reg.valid(hitbox_entity))
	{
		return true;
	}

	Circle hurtbox_shape = reg.get<C_Hurtbox>(hurtbox_entity).circle;
	Circle hitbox_shape = reg.get<C_Hitbox>(hitbox_entity).circle;

	v2 hitbox_pos = reg.get<C_Transform>(hitbox_entity)
						.get_global_transform()
						.pos;

	v2 hurtbox_pos = reg.get<C_Transform>(hurtbox_entity)
						 .get_local_transform()
						 .pos;

	hurtbox_shape.p += hurtbox_pos;
	hitbox_shape.p += hitbox_pos;

	if (circle_to_circle(hurtbox_shape, hitbox_shape))
	{
		// TODO: Signal for damage? Or just do it?
	}

	return true;
}

void handle_player_projectiles(entt::registry &reg, AabbGrid &grid)
{
	auto view = reg.view<C_Transform, C_Physics, C_Projectile>();
	for (auto e : view)
	{
		auto aabb = view.get<C_Physics>(e).aabb;
		auto pos = view.get<C_Transform>(e).get_local_transform().pos;
		aabb.min += pos;
		aabb.max += pos;

		aabb_grid_query(grid, aabb, on_hurtbox_to_hitbox, &e);
	}
}

bool hitbox_to_hitbox_resolve(
	Cute::AabbGridNode id,
	Cute::Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;
	entt::entity a = *(entt::entity *)fn_udata;
	entt::entity b = (entt::entity)(uint64_t)leaf_udata;

	if (a == b || !reg.valid(a) || !reg.valid(b))
	{
		return true;
	}

	auto &a_transform = reg.get<C_Transform>(a);
	auto &b_transform = reg.get<C_Transform>(b);

	// Copy, not reference
	auto a_hitbox = reg.get<C_Hitbox>(a).circle;
	auto b_hitbox = reg.get<C_Hitbox>(b).circle;

	auto a_pos = a_transform.get_local_transform().pos;
	auto b_pos = b_transform.get_local_transform().pos;

	a_hitbox.p += a_pos;
	b_hitbox.p += b_pos;

	Manifold manifold = {};
	circle_to_circle_manifold(a_hitbox, b_hitbox, &manifold);

	if (manifold.count > 0)
	{
		a_pos -= manifold.n * manifold.depths[0] * 0.5f;
		b_pos += manifold.n * manifold.depths[0] * 0.33f;

		a_transform.set_pos(a_pos);
		b_transform.set_pos(b_pos);
	}

	return true;
}

void handle_enemy_to_enemy_collisions(entt::registry &reg, AabbGrid &grid)
{
	auto view = reg.view<C_Transform, C_Enemy, C_Physics>();
	for (auto e : view)
	{
		auto aabb = view.get<C_Physics>(e).aabb;
		auto pos = view.get<C_Transform>(e).get_local_transform().pos;
		aabb.min += pos;
		aabb.max += pos;

		aabb_grid_query(grid, aabb, hitbox_to_hitbox_resolve, &e);
	}
}

void update_grid(entt::registry &reg, AabbGrid &grid)
{
	aabb_grid_clear(grid);

	auto player = reg.view<C_Player, C_Transform>().front();
	auto player_pos = reg.get<C_Transform>(player).get_global_transform().pos;
	aabb_grid_set_pos(grid, player_pos);

	auto view = reg.view<C_Transform, C_Enemy, C_Physics>();
	for (auto e : view)
	{
		auto aabb = view.get<C_Physics>(e).aabb;
		auto pos = view.get<C_Transform>(e).get_local_transform().pos;

		aabb.min += pos;
		aabb.max += pos;

		aabb_grid_insert(grid, aabb, (void *)e);
	}
}

void physics_system(entt::registry &reg, float dt)
{
	update_grid(reg, game.enemy_grid);

	handle_enemy_to_enemy_collisions(reg, game.enemy_grid);
	handle_player_projectiles(reg, game.enemy_grid);
}
