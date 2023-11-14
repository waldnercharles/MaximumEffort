#include "sys/physics_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_physics.h"
#include "cmp/c_projectile.h"
#include "cmp/c_transform.h"
#include "cute_aabb_grid.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

bool on_hurtbox_to_hitbox(
	AabbGridNode id,
	Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	const entt::entity hurtbox_entity = *static_cast<entt::entity *>(fn_udata);
	if (!reg.valid(hurtbox_entity))
	{
		return false;
	}

	const entt::entity hitbox_entity =
		static_cast<entt::entity>(reinterpret_cast<uint64_t>(leaf_udata));
	if (!reg.valid(hitbox_entity))
	{
		return true;
	}

	Circle hurtbox_shape = reg.get<C_Hurtbox>(hurtbox_entity).circle;
	Circle hitbox_shape = reg.get<C_Hitbox>(hitbox_entity).circle;

	const v2 hitbox_pos = reg.get<C_WorldTransform>(hitbox_entity).pos;
	const v2 hurtbox_pos = reg.get<C_WorldTransform>(hurtbox_entity).pos;

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
	const auto view = reg.view<C_WorldTransform, C_Physics, C_Projectile>();
	for (auto e : view)
	{
		auto aabb = view.get<C_Physics>(e).aabb;
		const auto pos = view.get<C_WorldTransform>(e).pos;
		aabb.min += pos;
		aabb.max += pos;

		aabb_grid_query(grid, aabb, on_hurtbox_to_hitbox, &e);
	}
}

bool hitbox_to_hitbox_resolve(
	AabbGridNode id,
	Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	//	auto &world = game.world;
	//	flecs::entity a = world->entity((flecs::id_t)fn_udata);
	//	flecs::entity b = world->entity((flecs::id_t)leaf_udata);
	//
	//	if (a == b || !a.is_alive() || !b.is_alive())
	//	{
	//		return true;
	//	}
	//
	//	// Copy, not reference
	//	auto a_hitbox = a.get<C_Hitbox>()->circle;
	//	auto b_hitbox = b.get<C_Hitbox>()->circle;
	//
	//	auto *a_world = a.get<C_WorldTransform>();
	//	auto *b_world = b.get<C_WorldTransform>();
	//
	//	auto a_pos = a_world->pos;
	//	auto b_pos = b_world->pos;
	//
	//	a_hitbox.p += a_pos;
	//	b_hitbox.p += b_pos;
	//
	//	Manifold manifold = {};
	//	circle_to_circle_manifold(a_hitbox, b_hitbox, &manifold);
	//
	//	if (manifold.count > 0)
	//	{
	//		auto *a_local = a.get<C_LocalTransform>();
	//		auto *b_local = b.get<C_LocalTransform>();
	//
	//		v2 delta = manifold.n * manifold.depths[0] * 0.33f;
	//
	//		//		a_local->pos -= delta;
	//		//		b_local->pos += delta;
	//	}

	return true;
}

void add_enemy_to_enemy_collision_system(flecs::world *world, AabbGrid &grid)
{
	world->system<const C_Enemy, const C_WorldTransform, const C_Physics>()
		.iter([=](flecs::iter &it,
				  const C_Enemy *,
				  const C_WorldTransform *w,
				  const C_Physics *p) {
			for (auto i : it)
			{
				auto aabb = p[i].aabb;
				auto pos = w[i].pos;
				aabb.min += pos;
				aabb.max += pos;

				aabb_grid_query(
					grid,
					aabb,
					hitbox_to_hitbox_resolve,
					(void *)it.entity(i).raw_id()
				);
			}
		});
}

void add_update_grid_system(flecs::world *world, AabbGrid &grid)
{
	world->system<const C_Enemy, const C_WorldTransform, const C_Physics>()
		.iter([=](flecs::iter &it,
				  const C_Enemy *e,
				  const C_WorldTransform *w,
				  const C_Physics *p) {
			aabb_grid_clear(grid);

			auto player_pos =
				world->lookup("player").get<C_WorldTransform>()->pos;

			aabb_grid_set_pos(grid, player_pos);

			for (auto i : it)
			{
				auto aabb = p[i].aabb;
				auto pos = w[i].pos;

				aabb.min += pos;
				aabb.max += pos;

				aabb_grid_insert(grid, aabb, (void *)it.entity(i).raw_id());
			}
		});
}

void add_physics_system(flecs::world *world, AabbGrid &grid)
{
	add_update_grid_system(world, grid);
	add_enemy_to_enemy_collision_system(world, grid);
	// handle_player_projectiles(reg, game.enemy_grid);
}
