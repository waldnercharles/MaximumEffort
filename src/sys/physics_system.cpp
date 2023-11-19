#include "sys/physics_system.h"
#include "cmp/enemy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"
#include "game.h"

#include <cute.h>

void PhysicsSystem::update_spatial_hash(World &world)
{
	grid.clear();
	auto players = world.view<PlayerComponent, TransformComponent>();
	auto player = players.front();
	auto player_pos = world.get<TransformComponent>(player)
						  .get_world_transform()
						  .pos;

	grid.pos = player_pos;

	auto view = world.view<EnemyComponent, TransformComponent, HitboxComponent>(
	);
	for (auto e : view)
	{
		auto p = view.get<TransformComponent>(e).get_world_transform().pos;
		auto c = view.get<HitboxComponent>(e).circle;
		c.p += p;

		Aabb aabb = cf_make_aabb(c.p - cf_v2(c.r, c.r), c.p + cf_v2(c.r, c.r));

		grid.add(aabb, e);
	}
}

void PhysicsSystem::handle_enemy_to_enemy_collisions(World &world)
{
	auto view = world.view<EnemyComponent, TransformComponent, HitboxComponent>(
	);
	for (auto a : view)
	{
		auto &a_transform = view.get<TransformComponent>(a);
		auto a_circle = view.get<HitboxComponent>(a).circle;
		a_circle.p += a_transform.get_world_transform().pos;

		Aabb a_aabb = cf_make_aabb(
			a_circle.p - cf_v2(a_circle.r, a_circle.r),
			a_circle.p + cf_v2(a_circle.r, a_circle.r)
		);

		grid.query(a_aabb, [&](Entity b) {
			if (a == b)
			{
				return true;
			}
			auto &b_transform = view.get<TransformComponent>(b);
			auto b_circle = view.get<HitboxComponent>(b).circle;
			b_circle.p += b_transform.get_world_transform().pos;

			Manifold manifold = {};
			cf_circle_to_circle_manifold(a_circle, b_circle, &manifold);

			if (manifold.count > 0)
			{
				v2 delta = manifold.n * manifold.depths[0] * 0.5f;

				a_transform -= delta;
				b_transform += delta;
			}

			return true;
		});
	}
}

PhysicsSystem::PhysicsSystem() : grid(480, 480, 16)
{
}

void PhysicsSystem::update(World &world)
{
	update_spatial_hash(world);

	handle_enemy_to_enemy_collisions(world);
	//	handle_player_projectiles(game.world, spatial_hash);
}
