#include "factories.h"
#include "cmp/c_debug_draw_circle.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_enemy_spawner.h"
#include "cmp/c_hitbox.h"
#include "cmp/c_hurtbox.h"
#include "cmp/c_input.h"
#include "cmp/c_movement.h"
#include "cmp/c_movement_behavior_follow_target.h"
#include "cmp/c_physics.h"
#include "cmp/c_player.h"
#include "cmp/c_projectile.h"
#include "cmp/c_sprite.h"
#include "cmp/c_transform.h"
#include "cmp/c_weapon.h"

using namespace Cute;

entt::entity
make_enemy_eyeball(entt::registry &reg, v2 pos, entt::entity target)
{
	const entt::entity e = reg.create();
	reg.emplace<C_Enemy>(e);

	auto &enemy_scene_node = reg.emplace<C_Transform>(e);
	enemy_scene_node.set_local_transform({pos});

	reg.emplace<C_Movement>(e);

	auto &physics = reg.emplace<C_Physics>(e);
	physics.aabb = make_aabb(V2(0, 0), 16, 16);

	auto &follow_behavior = reg.emplace<C_MovementBehavior_FollowTarget>(e);
	follow_behavior.entity = target;
	follow_behavior.speed = V2(20, 20);
	follow_behavior.face_target = true;

	auto &hitbox = reg.emplace<C_Hitbox>(e);
	hitbox.circle = make_circle(V2(8, 8), 8);

	auto &hurtbox = reg.emplace<C_Hurtbox>(e);
	hurtbox.circle = make_circle(V2(8, 8), 6);

	auto &sprite = reg.emplace<C_Sprite>(e, make_sprite("eyeball.ase"));
	sprite.layer = 50;

	return e;
}

entt::entity make_player(entt::registry &reg)
{
	const entt::entity e = reg.create();
	reg.emplace<C_Transform>(e);
	reg.emplace<C_Player>(e);

	auto &i = reg.emplace<C_Input>(e);
	i.speed = V2(45, 45);

	reg.emplace<C_Movement>(e);

	auto &s = reg.emplace<C_Sprite>(e, make_sprite("character.ase"));
	s.layer = 100;

	return e;
}

entt::entity make_enemy_spawner(
	entt::registry &reg,
	entt::entity parent,
	float rate,
	EnemyType spawn_type
)
{
	const entt::entity e = reg.create();
	auto &player_scene_node = reg.get<C_Transform>(parent);
	auto &spawner_scene_node = reg.emplace<C_Transform>(e);
	player_scene_node.add_child(&spawner_scene_node);

	auto &s = reg.emplace<C_EnemySpawner>(e);
	s.rate = rate;
	s.entity_type = spawn_type;

	return e;
}

entt::entity make_weapon_boomerang(entt::registry &reg, entt::entity parent)
{
	const entt::entity e = reg.create();

	auto &parent_scene_node = reg.get<C_Transform>(parent);
	auto &child_scene_node = reg.emplace<C_Transform>(e);

	parent_scene_node.add_child(&child_scene_node);

	auto &w = reg.emplace<C_Weapon>(e);
	w.rate = 0.3f;
	w.weapon_type = WEAPON_BOOMERANG;
	w.targeting_radius = 64.f;
	w.target_type = TARGET_CLOSEST_ENEMY;

	auto &dbg = reg.emplace<C_DebugDrawCircle>(e);
	dbg.color = make_color(0, 0.5f, 1, 0.5f);
	dbg.circle = make_circle(V2(0, 0), w.targeting_radius);

	reg.emplace<C_Movement>(e);

	return e;
}