#include "sys/physics_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_player.h"
#include "cmp/c_transform.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

void PhysicsSystem::update_spatial_hash(entt::registry &reg)
{
	grid.clear();
	auto players = reg.view<C_Player, C_Transform>();
	auto player = players.front();
	auto player_pos = reg.get<C_Transform>(player).get_global_transform().pos;

	grid.pos = player_pos;

	auto view = reg.view<C_Enemy, C_Transform, C_Hitbox>();
	for (auto e : view)
	{
		auto p = view.get<C_Transform>(e).get_global_transform().pos;
		auto c = view.get<C_Hitbox>(e).circle;
		c.p += p;

		Aabb aabb = make_aabb(c.p - V2(c.r, c.r), c.p + V2(c.r, c.r));

		grid.add(aabb, e);
	}
}

void PhysicsSystem::handle_enemy_to_enemy_collisions(entt::registry &reg)
{
	using namespace Cute;

	auto view = reg.view<C_Enemy, C_Transform, C_Hitbox>();
	for (auto a : view)
	{
		auto &a_transform = view.get<C_Transform>(a);
		auto a_circle = view.get<C_Hitbox>(a).circle;
		a_circle.p += a_transform.get_global_transform().pos;

		Aabb a_aabb = make_aabb(
			a_circle.p - V2(a_circle.r, a_circle.r),
			a_circle.p + V2(a_circle.r, a_circle.r)
		);

		grid.query(a_aabb, [&](entt::entity b) {
			if (a == b)
			{
				return true;
			}
			auto &b_transform = view.get<C_Transform>(b);
			auto b_circle = view.get<C_Hitbox>(b).circle;
			b_circle.p += b_transform.get_global_transform().pos;

			Manifold manifold = {};
			circle_to_circle_manifold(a_circle, b_circle, &manifold);

			if (manifold.count > 0)
			{
				Cute::v2 delta = manifold.n * manifold.depths[0] * 0.5f;

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
	update_spatial_hash(game.reg);

	handle_enemy_to_enemy_collisions(game.reg);
	//	handle_player_projectiles(game.reg, spatial_hash);
}
