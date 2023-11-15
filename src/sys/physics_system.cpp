#include "sys/physics_system.h"
#include "array_algorithms.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_physics.h"
#include "cmp/c_projectile.h"
#include "cmp/c_transform.h"
#include "cute_aabb_grid.h"
#include "cute_ext.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

//bool on_hurtbox_to_hitbox(
//	AabbGridNode id,
//	Aabb aabb,
//	void *leaf_udata,
//	void *fn_udata
//)
//{
//	auto &reg = game.reg;
//
//	const entt::entity hurtbox_entity = *static_cast<entt::entity *>(fn_udata);
//	if (!reg.valid(hurtbox_entity))
//	{
//		return false;
//	}
//
//	const entt::entity hitbox_entity =
//		static_cast<entt::entity>(reinterpret_cast<uint64_t>(leaf_udata));
//	if (!reg.valid(hitbox_entity))
//	{
//		return true;
//	}
//
//	Circle hurtbox_shape = reg.get<C_Hurtbox>(hurtbox_entity).circle;
//	Circle hitbox_shape = reg.get<C_Hitbox>(hitbox_entity).circle;
//
//	const v2 hitbox_pos = reg.get<C_WorldTransform>(hitbox_entity).pos;
//	const v2 hurtbox_pos = reg.get<C_WorldTransform>(hurtbox_entity).pos;
//
//	hurtbox_shape.p += hurtbox_pos;
//	hitbox_shape.p += hitbox_pos;
//
//	if (circle_to_circle(hurtbox_shape, hitbox_shape))
//	{
//		// TODO: Signal for damage? Or just do it?
//	}
//
//	return true;
//}
//
//void handle_player_projectiles(entt::registry &reg, AabbGrid &grid)
//{
//	const auto view = reg.view<C_WorldTransform, C_Physics, C_Projectile>();
//	for (auto e : view)
//	{
//		auto aabb = view.get<C_Physics>(e).aabb;
//		const auto pos = view.get<C_WorldTransform>(e).pos;
//		aabb.min += pos;
//		aabb.max += pos;
//
//		aabb_grid_query(grid, aabb, on_hurtbox_to_hitbox, &e);
//	}
//}
//
//bool hitbox_to_hitbox_resolve(
//	AabbGridNode id,
//	Aabb aabb,
//	void *leaf_udata,
//	void *fn_udata
//)
//{
//	//	auto &world = game.world;
//	//	flecs::entity a = world->entity((flecs::id_t)fn_udata);
//	//	flecs::entity b = world->entity((flecs::id_t)leaf_udata);
//	//
//	//	if (a == b || !a.is_alive() || !b.is_alive())
//	//	{
//	//		return true;
//	//	}
//	//
//	//	// Copy, not reference
//	//	auto a_hitbox = a.get<C_Hitbox>()->circle;
//	//	auto b_hitbox = b.get<C_Hitbox>()->circle;
//	//
//	//	auto *a_world = a.get<C_WorldTransform>();
//	//	auto *b_world = b.get<C_WorldTransform>();
//	//
//	//	auto a_pos = a_world->pos;
//	//	auto b_pos = b_world->pos;
//	//
//	//	a_hitbox.p += a_pos;
//	//	b_hitbox.p += b_pos;
//	//
//	//	Manifold manifold = {};
//	//	circle_to_circle_manifold(a_hitbox, b_hitbox, &manifold);
//	//
//	//	if (manifold.count > 0)
//	//	{
//	//		auto *a_local = a.get<C_LocalTransform>();
//	//		auto *b_local = b.get<C_LocalTransform>();
//	//
//	//		v2 delta = manifold.n * manifold.depths[0] * 0.33f;
//	//
//	//		//		a_local->pos -= delta;
//	//		//		b_local->pos += delta;
//	//	}
//
//	return true;
//}
//
////void add_enemy_to_enemy_collision_system(flecs::world *world, AabbGrid &grid)
////{
////	world->system<const C_Enemy, const C_WorldTransform, const C_Physics>()
////		.iter([=](flecs::iter &it,
////				  const C_Enemy *,
////				  const C_WorldTransform *w,
////				  const C_Physics *p) {
////			for (auto i : it)
////			{
////				auto aabb = p[i].aabb;
////				auto pos = w[i].pos;
////				aabb.min += pos;
////				aabb.max += pos;
////
////				aabb_grid_query(
////					grid,
////					aabb,
////					hitbox_to_hitbox_resolve,
////					(void *)it.entity(i).raw_id()
////				);
////			}
////		});
////}
//
////void add_update_grid_system(flecs::world *world, AabbGrid &grid)
////{
////	world->system<const C_Enemy, const C_WorldTransform, const C_Physics>()
////		.iter([=](flecs::iter &it,
////				  const C_Enemy *e,
////				  const C_WorldTransform *w,
////				  const C_Physics *p) {
////			aabb_grid_clear(grid);
////
////			auto player_pos =
////				world->lookup("player").get<C_WorldTransform>()->pos;
////
////			aabb_grid_set_pos(grid, player_pos);
////
////			for (auto i : it)
////			{
////				auto aabb = p[i].aabb;
////				auto pos = w[i].pos;
////
////				aabb.min += pos;
////				aabb.max += pos;
////
////				aabb_grid_insert(grid, aabb, (void *)it.entity(i).raw_id());
////			}
////		});
////}
//
//
//void update_grid(flecs::world *world, EcsAabbGrid *grid)
//{
//	grid->map.ensure_count(
//		grid->w / grid->region_size * grid->h / grid->region_size
//	);
//
//	world->system<const C_Physics, const C_LocalTransform>("update_grid")
//		.iter([world, grid](
//				  flecs::iter &it,
//				  const C_Physics *p,
//				  const C_LocalTransform *l
//			  ) {
//			for (auto i : it)
//			{
//				auto e = it.entity(i);
//
//				v2 half_extents = V2((float)grid->w, (float)grid->h) * 0.5f;
//
//				int max_tiles_x = grid->w / grid->region_size;
//				int max_tiles_y = grid->h / grid->region_size;
//
//				Aabb relative_aabb = p[i].aabb;
//				relative_aabb.min += half_extents - grid->pos + l[i].pos;
//				relative_aabb.max += half_extents - grid->pos + l[i].pos;
//
//				int min_x = (int)relative_aabb.min.x / grid->region_size;
//				int min_y = (int)relative_aabb.min.y / grid->region_size;
//
//				int max_x = (int)relative_aabb.max.x / grid->region_size;
//				int max_y = (int)relative_aabb.max.y / grid->region_size;
//
//				if (min_x < 0 || min_y < 0 || max_x >= max_tiles_x ||
//					max_y >= max_tiles_y)
//				{
//					return;
//				}
//
//				for (int x = min_x; x <= max_x; x++)
//				{
//					for (int y = min_y; y <= max_y; y++)
//					{
//						int index = y * max_tiles_x + x;
//
//						if (grid->map[index] == flecs::entity::null())
//						{
//							grid->map[index] = world->entity();
//						}
//
//						e.add<Region>(grid->map[index]);
//					}
//				}
//			}
//		});
//}
//
//void add_enemy_to_enemy_collision_system(flecs::world *world, EcsAabbGrid *grid)
//{
//	auto grouped_query = world
//							 ->query_builder<
//								 const C_Enemy,
//								 C_LocalTransform,
//								 C_WorldTransform,
//								 const C_Hitbox>("enemy_to_enemy_cell_query")
//							 .group_by<Region>([](flecs::world_t *world,
//												  flecs::table_t *table,
//												  flecs::id_t id,
//												  void *ctx) {
//								 flecs::entity_t result = 0;
//								 ecs_id_t pair = 0;
//
//								 if (ecs_search(
//										 world,
//										 table,
//										 ecs_pair(id, EcsWildcard),
//										 &pair
//									 ) != -1)
//								 {
//									 result = ecs_pair_second(world, pair);
//								 }
//
//								 return result;
//							 })
//							 .build();
//
//	world
//		->system<
//			const C_Enemy,
//			C_LocalTransform,
//			C_WorldTransform,
//			const C_Hitbox,
//			const C_Physics>("enemy_to_enemy")
//		.each([=](flecs::entity a,
//				  const C_Enemy,
//				  C_LocalTransform &a_local,
//				  C_WorldTransform &a_world,
//				  C_Hitbox a_hitbox,
//				  const C_Physics &a_physics) {
//			static Array<flecs::entity> visited;
//			visited.clear();
//
//			v2 half_extents = V2((float)grid->w, (float)grid->h) * 0.5f;
//
//			int max_tiles_x = grid->w / grid->region_size;
//			int max_tiles_y = grid->h / grid->region_size;
//
//			Aabb relative_aabb = a_physics.aabb;
//			relative_aabb.min += half_extents - grid->pos + a_local.pos;
//			relative_aabb.max += half_extents - grid->pos + a_local.pos;
//
//			int min_x = (int)relative_aabb.min.x / grid->region_size;
//			int min_y = (int)relative_aabb.min.y / grid->region_size;
//
//			int max_x = (int)relative_aabb.max.x / grid->region_size;
//			int max_y = (int)relative_aabb.max.y / grid->region_size;
//
//			if (min_x < 0 || min_y < 0 || max_x >= max_tiles_x ||
//				max_y >= max_tiles_y)
//			{
//				return;
//			}
//
//			a_hitbox.circle.p += a_local.pos;
//
//			for (int x = min_x; x <= max_x; x++)
//			{
//				for (int y = min_y; y <= max_y; y++)
//				{
//					int index = y * max_tiles_x + x;
//					grouped_query.iter()
//						.set_group(grid->map[index])
//						.each([a, a_hitbox, &a_local, &a_world](
//								  flecs::entity b,
//								  const C_Enemy,
//								  C_LocalTransform &b_local,
//								  C_WorldTransform &b_world,
//								  C_Hitbox b_hitbox
//							  ) {
//							if (a == b ||
//								arr_contains(visited.begin(), visited.end(), b))
//							{
//								return;
//							}
//							visited.add(b);
//
//							b_hitbox.circle.p += b_local.pos;
//
//							Manifold manifold = {};
//							circle_to_circle_manifold(
//								a_hitbox.circle,
//								b_hitbox.circle,
//								&manifold
//							);
//
//							if (manifold.count > 0)
//							{
//								v2 delta =
//									manifold.n * manifold.depths[0] * 0.5f;
//
//								a_local.pos -= delta;
//								b_local.pos += delta;
//
//								a_world.pos -= delta;
//								b_world.pos += delta;
//							}
//						});
//				}
//			}
//		});
//}
//
//void add_physics_system()
//{
//	update_grid(game.world, &game.enemy_grid);
//	add_enemy_to_enemy_collision_system(game.world, &game.enemy_grid);
//	// handle_player_projectiles(reg, game.enemy_grid);
//}
//
////void physics_system(flecs::world *world)
////{
////	static EcsAabbGrid enemies = {320, 320, {0, 0}, 32, {}};
////	update_grid(world);
////	add_enemy_to_enemy_collision_system(world, enemies);
////}

void physics_system(flecs::world *world)
{
	auto q = std::make_shared<
		flecs::query<C_Enemy, C_LocalTransform, C_WorldTransform, C_Hitbox>>(
		world->query<C_Enemy, C_LocalTransform, C_WorldTransform, C_Hitbox>()
	);

	world
		->system<C_Enemy, C_LocalTransform, C_WorldTransform, C_Hitbox>(
			"physics"
		)
		.run([](flecs::iter_t *it) {
			int count = 0;
			it->ctx = &count;
			while (ecs_query_next(it))
			{
				it->callback(it);
			}
		})
		.each([=](flecs::iter &it,
				  size_t,
				  C_Enemy,
				  C_LocalTransform &a_local,
				  C_WorldTransform &a_world,
				  C_Hitbox a_hitbox) {
			int *count = it.ctx<int>();

			auto a_circle = a_hitbox.circle;
			a_circle.p += a_world.pos;

			auto a_cell = std::pair<s16, s16>(
				(int)a_world.pos.x >> 4,
				(int)a_world.pos.y >> 4
			);

			(*count)++;

			q->page(*count, 0).each([&a_circle, a_cell, &a_local, &a_world](
										C_Enemy,
										C_LocalTransform &b_local,
										C_WorldTransform &b_world,
										C_Hitbox b_hitbox
									) {
				auto b_circle = b_hitbox.circle;
				b_circle.p += b_world.pos;

				auto b_cell = std::pair<s16, s16>(
					(int)b_world.pos.x >> 4,
					(int)b_world.pos.y >> 4
				);

				auto delta_x = std::abs((a_cell.first - b_cell.first));
				auto delta_y = std::abs((a_cell.second - b_cell.second));

				if (delta_x <= 1 && delta_y <= 1)
				{
					Manifold manifold = {};
					circle_to_circle_manifold(a_circle, b_circle, &manifold);

					if (manifold.count > 0)
					{
						v2 delta = manifold.n * manifold.depths[0] * 0.5f;

						a_local.pos -= delta;
						b_local.pos += delta;

						a_world.pos -= delta;
						b_world.pos += delta;
					}
				}
			});
		});
}