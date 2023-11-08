#include "sys/physics_system.h"
#include "cmp/enemy_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/physics_component.h"
#include "cmp/projectile_component.h"
#include "cmp/scene_node_component.h"
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
	Circle projectile_hurtbox = reg.get<HurtboxComponent>(*projectile).circle;
	Circle enemy_hitbox = reg.get<HitboxComponent>(enemy).circle;

	v2 projectile_pos = reg.get<SceneNodeComponent>(*projectile)
							.get_global_transform()
							.pos;

	v2 enemy_pos = reg.get<SceneNodeComponent>(enemy)
					   .get_global_transform()
					   .pos;

	projectile_hurtbox.p += projectile_pos;
	enemy_hitbox.p += enemy_pos;

	if (circle_to_circle(projectile_hurtbox, enemy_hitbox))
	{
		//		if (*projectile != entt::null && reg.valid(*projectile))
		//		{
		//			reg.destroy(*projectile);
		//			*projectile = entt::null;
		//		}
		reg.destroy(enemy);
		aabb_tree_remove(game.enemy_aabb_tree, leaf);
	}

	return true;
}

void update_enemy_aabb_tree(entt::registry &reg, AabbTree &tree)
{
	auto view = reg.view<EnemyComponent, SceneNodeComponent, PhysicsComponent>(
	);
	for (auto e : view)
	{
		auto &scene_node = view.get<SceneNodeComponent>(e);
		auto enemy_pos = scene_node.get_global_transform().pos;

		auto &p = view.get<PhysicsComponent>(e);

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
	auto view = reg.view<
		ProjectileComponent,
		SceneNodeComponent,
		PhysicsComponent>();

	for (auto e : view)
	{
		auto &scene_node = view.get<SceneNodeComponent>(e);
		auto projectile_pos = scene_node.get_global_transform().pos;

		auto &p = view.get<PhysicsComponent>(e);

		Cute::Aabb aabb = make_aabb(
			p.aabb.min + projectile_pos,
			p.aabb.max + projectile_pos
		);
		aabb_tree_query(tree, on_projectile_hit, aabb, &e);
	}
}

void physics_system(entt::registry &reg, float dt)
{
	update_enemy_aabb_tree(reg, game.enemy_aabb_tree);
	handle_player_projectiles(reg, game.enemy_aabb_tree);
}
