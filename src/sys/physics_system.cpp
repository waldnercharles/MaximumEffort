#include "sys/physics_system.h"
#include "cmp/enemy.h"
#include "cmp/hitbox.h"
#include "cmp/player.h"
#include "cmp/transform.h"
#include "game.h"

#include <cute.h>

void PhysicsSystem::update_spatial_hash(World &world)
{
	grid.clear();
	auto players = world.view<Player, Transform>();
	auto player = players.front();
	auto player_pos = world.get<Transform>(player).get_global_transform().pos;

	grid.pos = player_pos;

	auto view = world.view<Enemy, Transform, Hitbox>();
	for (auto e : view)
	{
		auto p = view.get<Transform>(e).get_global_transform().pos;
		auto c = view.get<Hitbox>(e).circle;
		c.p += p;

		Aabb aabb = cf_make_aabb(c.p - cf_v2(c.r, c.r), c.p + cf_v2(c.r, c.r));

		grid.add(aabb, e);
	}
}

void PhysicsSystem::handle_enemy_to_enemy_collisions(World &world)
{
	auto view = world.view<Enemy, Transform, Hitbox>();
	for (auto a : view)
	{
		auto &a_transform = view.get<Transform>(a);
		auto a_circle = view.get<Hitbox>(a).circle;
		a_circle.p += a_transform.get_global_transform().pos;

		Aabb a_aabb = cf_make_aabb(
			a_circle.p - cf_v2(a_circle.r, a_circle.r),
			a_circle.p + cf_v2(a_circle.r, a_circle.r)
		);

		grid.query(a_aabb, [&](Entity b) {
			if (a == b)
			{
				return true;
			}
			auto &b_transform = view.get<Transform>(b);
			auto b_circle = view.get<Hitbox>(b).circle;
			b_circle.p += b_transform.get_global_transform().pos;

			Manifold manifold = {};
			cf_circle_to_circle_manifold(a_circle, b_circle, &manifold);

			if (manifold.count > 0)
			{
				v2 delta = manifold.n * manifold.depths[0] * 0.5f;

				a_transform.offset(-delta);
				b_transform.offset(delta);
			}

			return true;
		});
	}
}

PhysicsSystem::PhysicsSystem() : grid(480, 480, 16)
{
}

void PhysicsSystem::update(f32 delta_time)
{
	update_spatial_hash(game.world);

	handle_enemy_to_enemy_collisions(game.world);
	//	handle_player_projectiles(game.world, spatial_hash);
}
