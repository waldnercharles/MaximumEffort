#include "sys/physics_system.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_movement.h"
#include "cmp/c_physics.h"
#include "cmp/c_projectile.h"
#include "cmp/c_transform.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

bool on_projectile_hit(
	CF_Leaf leaf,
	CF_Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	entt::entity enemy = (entt::entity)(u64)leaf_udata;
	if (!reg.valid(enemy))
	{
		return true;
	}

	entt::entity *projectile = (entt::entity *)fn_udata;
	if (!reg.valid(*projectile))
	{
		return false;
	}

	// Intentional Copy
	Circle projectile_hurtbox = reg.get<C_Hurtbox>(*projectile).circle;
	Circle enemy_hitbox = reg.get<C_Hitbox>(enemy).circle;

	v2 projectile_pos = reg.get<C_Transform>(*projectile)
							.get_global_transform()
							.pos;

	v2 enemy_pos = reg.get<C_Transform>(enemy).get_local_transform().pos;

	projectile_hurtbox.p += projectile_pos;
	enemy_hitbox.p += enemy_pos;

	if (circle_to_circle(projectile_hurtbox, enemy_hitbox))
	{
		reg.destroy(enemy);
		aabb_tree_remove(game.enemy_aabb_tree, leaf);
	}

	return true;
}

void update_enemy_aabb_tree(entt::registry &reg, AabbTree &tree)
{
	auto view = reg.view<C_Enemy, C_Transform, C_Physics>();
	for (auto e : view)
	{
		auto &scene_node = view.get<C_Transform>(e);
		auto enemy_pos = scene_node.get_local_transform().pos;

		auto &p = view.get<C_Physics>(e);

		Cute::Aabb aabb = make_aabb(
			p.aabb.min + enemy_pos,
			p.aabb.max + enemy_pos
		);

		if (p.leaf.id != -1)
		{
			aabb_tree_update_leaf(tree, p.leaf, aabb);
		}
		else
		{
			p.leaf = aabb_tree_insert(tree, aabb, (void *)e);
		}
	}
}

void handle_player_projectiles(entt::registry &reg, AabbTree &tree)
{
	auto view = reg.view<C_Projectile, C_Transform, C_Physics>();

	for (auto e : view)
	{
		auto &transform = view.get<C_Transform>(e);
		auto projectile_pos = transform.get_global_transform().pos;

		auto &p = view.get<C_Physics>(e);

		Cute::Aabb aabb = make_aabb(
			p.aabb.min + projectile_pos,
			p.aabb.max + projectile_pos
		);
		aabb_tree_query(tree, on_projectile_hit, aabb, &e);
	}
}

bool on_enemy_to_enemy_collision(
	CF_Leaf leaf,
	CF_Aabb aabb,
	void *leaf_udata,
	void *fn_udata
)
{
	auto &reg = game.reg;

	entt::entity a = (entt::entity)(u64)leaf_udata;
	entt::entity b = (entt::entity)(u64)fn_udata;
	if (a == b || !reg.valid(a) || !reg.valid(b))
	{
		return true;
	}

	auto &a_transform = reg.get<C_Transform>(a);
	auto &b_transform = reg.get<C_Transform>(b);

	// Copy, not reference
	auto a_hitbox = reg.get<C_Hitbox>(a).circle;
	auto b_hitbox = reg.get<C_Hitbox>(b).circle;

	a_hitbox.p += a_transform.get_local_transform().pos;
	b_hitbox.p += b_transform.get_local_transform().pos;

	Manifold manifold = {};
	circle_to_circle_manifold(a_hitbox, b_hitbox, &manifold);

	if (manifold.count > 0)
	{
		for (int i = 0; i < manifold.count; i++)
		{
			auto a_local_pos = a_transform.get_local_transform().pos;
			auto b_local_pos = b_transform.get_local_transform().pos;

			a_local_pos -= manifold.n * manifold.depths[i] * 0.125;
			b_local_pos += manifold.n * manifold.depths[i] * 0.125;

			a_transform.set_pos(a_local_pos);
			b_transform.set_pos(b_local_pos);
		}
	}

	return true;
}

struct EnemyCollisionThreadData
{
	entt::registry &reg;
	AabbTree &tree;
	int offset;
};

int handle_enemy_collisions_thread(void *udata)
{
	EnemyCollisionThreadData *data = (EnemyCollisionThreadData *)udata;

	int i = 0;
	auto view = data->reg.view<C_Transform, C_Enemy, C_Physics>();

	for (auto e : view)
	{
		if ((i++ % 4) != data->offset)
		{
			continue;
		}

		auto &p = view.get<C_Physics>(e);
		auto pos = view.get<C_Transform>(e).get_local_transform().pos;

		Cute::Aabb aabb = make_aabb(p.aabb.min + pos, p.aabb.max + pos);
		aabb_tree_query(
			data->tree,
			on_enemy_to_enemy_collision,
			aabb,
			(void *)e
		);
	}

	return 0;
}

void handle_enemy_to_enemy_collisions(entt::registry &reg, AabbTree &tree)
{
	auto view = reg.view<C_Transform, C_Enemy, C_Physics>();
	for (auto e : view)
	{
		auto &p = view.get<C_Physics>(e);
		auto pos = view.get<C_Transform>(e).get_local_transform().pos;

		Cute::Aabb aabb = make_aabb(p.aabb.min + pos, p.aabb.max + pos);
		aabb_tree_query(tree, on_enemy_to_enemy_collision, aabb, (void *)e);
	}

	//	for (auto a : view)
	//	{
	//		auto &a_transform = view.get<C_Transform>(a);
	//		auto a_hitbox = view.get<C_Hitbox>(a).circle;
	//		a_hitbox.p += a_transform.get_local_transform().pos;
	//		auto &a_movement = view.get<C_Movement>(a);
	//		for (auto b : view)
	//		{
	//			if (a == b)
	//			{
	//				continue;
	//			}
	//
	//			auto &b_transform = view.get<C_Transform>(b);
	//			auto b_hitbox = view.get<C_Hitbox>(b).circle;
	//			b_hitbox.p += b_transform.get_local_transform().pos;
	//			auto &b_movement = view.get<C_Movement>(b);
	//
	//			Manifold m = {};
	//			circle_to_circle_manifold(a_hitbox, b_hitbox, &m);
	//			if (m.count)
	//			{
	//				auto a_local_pos = a_transform.get_local_transform().pos;
	//				auto b_local_pos = b_transform.get_local_transform().pos;
	//
	//				v2 push = m.n * 0.5f * m.depths[0];
	//
	//				//				a_movement.vel -= push * 30;
	//				//				b_movement.vel += push * 30;
	//
	//				a_local_pos -= push;
	//				b_local_pos += push;
	//
	//				a_transform.set_pos(a_local_pos);
	//				b_transform.set_pos(b_local_pos);
	//			}
	//		}
	//	}
}

void physics_system(entt::registry &reg, float dt)
{
	update_enemy_aabb_tree(reg, game.enemy_aabb_tree);

	handle_enemy_to_enemy_collisions(reg, game.enemy_aabb_tree);
	//	handle_player_projectiles(reg, game.enemy_aabb_tree);
}
