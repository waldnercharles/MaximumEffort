#include "sys/physics_system.h"
#include "cmp/enemy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/movement_component.h"
#include "cmp/physics_component.h"
#include "cmp/projectile_component.h"
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
	entt::entity projectile = (entt::entity)(u64)fn_udata;

	auto *projectile_hurtbox = reg.try_get<HurtboxComponent>(projectile);
	auto *enemy_hitbox = reg.try_get<HitboxComponent>(enemy);

	if (!projectile_hurtbox || !enemy_hitbox)
	{
		return true;
	}

	if (circle_to_circle(projectile_hurtbox->circle, enemy_hitbox->circle))
	{
		reg.destroy(enemy);
		aabb_tree_remove(game.enemy_aabb_tree, leaf);
	}

	return true;
}

void update_enemy_aabb_tree(entt::registry &reg, AabbTree &tree)
{
	auto view = reg.view<EnemyComponent, MovementComponent, PhysicsComponent>();
	for (auto e : view)
	{
		auto &m = view.get<MovementComponent>(e);
		auto &p = view.get<PhysicsComponent>(e);

		Cute::Aabb aabb = make_aabb(p.aabb.min + m.pos, p.aabb.max + m.pos);

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
	auto view = reg.view<
		ProjectileComponent,
		MovementComponent,
		PhysicsComponent>();

	for (auto e : view)
	{
		auto &m = view.get<MovementComponent>(e);
		auto &p = view.get<PhysicsComponent>(e);

		Cute::Aabb aabb = make_aabb(p.aabb.min + m.pos, p.aabb.max + m.pos);
		aabb_tree_query(tree, on_projectile_hit, aabb, (void *)e);
	}
}

void physics_system(entt::registry &reg, float dt)
{
	update_enemy_aabb_tree(reg, game.enemy_aabb_tree);
	handle_player_projectiles(reg, game.enemy_aabb_tree);
}
