#include "factories.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/enemy_component.h"
#include "cmp/follow_target_behavior_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/physics_component.h"
#include "cmp/player_component.h"
#include "cmp/projectile_component.h"
#include "cmp/scene_node_component.h"
#include "cmp/spawner_component.h"
#include "cmp/sprite_component.h"
#include "cmp/weapon_component.h"

using namespace Cute;

entt::entity
make_enemy_eyeball(entt::registry &reg, v2 pos, entt::entity target)
{
	const entt::entity e = reg.create();
	reg.emplace<EnemyComponent>(e);

	auto &enemy_scene_node = reg.emplace<SceneNodeComponent>(e);
	enemy_scene_node.set_local_transform({pos});

	reg.emplace<MovementComponent>(e);

	auto &physics = reg.emplace<PhysicsComponent>(e);
	physics.aabb = make_aabb(V2(0, 0), 16, 16);

	auto &follow_behavior = reg.emplace<FollowTargetBehaviorComponent>(e);
	follow_behavior.entity = target;
	follow_behavior.speed = V2(20, 20);
	follow_behavior.face_target = true;

	auto &hitbox = reg.emplace<HitboxComponent>(e);
	hitbox.circle = make_circle(V2(8, 8), 8);

	auto &hurtbox = reg.emplace<HurtboxComponent>(e);
	hurtbox.circle = make_circle(V2(8, 8), 6);

	auto &sprite = reg.emplace<SpriteComponent>(e, make_sprite("eyeball.ase"));
	sprite.layer = 50;

	return e;
}

entt::entity make_player(entt::registry &reg)
{
	const entt::entity e = reg.create();
	reg.emplace<SceneNodeComponent>(e);
	reg.emplace<PlayerComponent>(e);

	auto &i = reg.emplace<InputComponent>(e);
	i.speed = V2(45, 45);

	reg.emplace<MovementComponent>(e);

	auto &s = reg.emplace<SpriteComponent>(e, make_sprite("character.ase"));
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
	auto &player_scene_node = reg.get<SceneNodeComponent>(parent);
	auto &spawner_scene_node = reg.emplace<SceneNodeComponent>(e);
	player_scene_node.add_child(&spawner_scene_node);

	auto &s = reg.emplace<SpawnerComponent>(e);
	s.rate = rate;
	s.entity_type = spawn_type;


	return e;
}

entt::entity make_weapon_boomerang(entt::registry &reg, entt::entity parent)
{
	const entt::entity e = reg.create();

	auto &parent_scene_node = reg.get<SceneNodeComponent>(parent);
	auto &child_scene_node = reg.emplace<SceneNodeComponent>(e);

	parent_scene_node.add_child(&child_scene_node);

	auto &w = reg.emplace<WeaponComponent>(e);
	w.rate = 0.05f;
	w.weapon_type = WEAPON_BOOMERANG;
	w.targeting_radius = 64.f;
	w.target_type = TARGET_CLOSEST_ENEMY;

	auto &dbg = reg.emplace<DebugDrawCircleComponent>(e);
	dbg.color = make_color(0, 0.5f, 1, 0.5f);
	dbg.circle = make_circle(V2(0, 0), w.targeting_radius);

	reg.emplace<MovementComponent>(e);

	return e;
}