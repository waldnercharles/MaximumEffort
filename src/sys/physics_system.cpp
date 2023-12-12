#include "sys/physics_system.h"
#include "cmp/enemy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/physics_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"

#include <cute.h>

void update_aabb_grid(World &w, AabbGrid<Entity> &grid)
{
	grid.clear();
	auto players = w.view<PlayerComponent, TransformComponent>();
	auto player = players.front();
	auto player_pos =
		w.get<TransformComponent>(player).get_world_transform().pos;

	grid.pos = player_pos;

	auto view = w.view<EnemyComponent, TransformComponent, HitboxComponent>();
	for (auto e : view)
	{
		auto p = view.get<TransformComponent>(e).get_world_transform().pos;
		auto c = view.get<HitboxComponent>(e).circle;
		c.p += p;

		Aabb aabb = cf_make_aabb(c.p - cf_v2(c.r, c.r), c.p + cf_v2(c.r, c.r));

		grid.add(aabb, e);
	}
}

void PhysicsSystem::handle_enemy_to_enemy_collisions(World &w)
{
	auto view = w.view<EnemyComponent, TransformComponent, PhysicsComponent>();
	for (auto a : view)
	{
		auto &a_transform = view.get<TransformComponent>(a);
		auto &a_physics = view.get<PhysicsComponent>(a);

		auto a_circle = a_physics.shape;
		a_circle.p += a_transform.get_world_transform().pos;

		float a_weight = a_physics.mass;

		Aabb a_aabb = cf_make_aabb(
			a_circle.p - cf_v2(a_circle.r, a_circle.r),
			a_circle.p + cf_v2(a_circle.r, a_circle.r)
		);

		enemy_grid.query(a_aabb, [&](Entity b) {
			if (a == b)
			{
				return true;
			}
			auto &b_transform = view.get<TransformComponent>(b);
			auto &b_physics = view.get<PhysicsComponent>(b);

			auto b_circle = b_physics.shape;
			b_circle.p += b_transform.get_world_transform().pos;

			float b_weight = b_physics.mass;

			Manifold manifold = {};
			cf_circle_to_circle_manifold(a_circle, b_circle, &manifold);

			if (manifold.count > 0)
			{
				v2 normal = manifold.n;
				float depth = manifold.depths[0];
				float weight = (a_weight + b_weight);

				a_transform -= normal * depth * (b_weight / weight);
				b_transform += normal * depth * (a_weight / weight);
			}

			return true;
		});
	}
}

PhysicsSystem::PhysicsSystem(AabbGrid<Entity> &enemy_grid)
	: enemy_grid(enemy_grid)
{
}

void PhysicsSystem::update(World &world)
{
	update_aabb_grid(world, enemy_grid);
	handle_enemy_to_enemy_collisions(world);
}
