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
#include "cmp/c_sprite.h"
#include "cmp/c_transform.h"
#include "cmp/c_weapon.h"

#include <flecs.h>

using namespace Cute;

flecs::entity make_enemy_eyeball(flecs::world *world, v2 pos)
{
	flecs::entity e = world->entity();
	e.add<C_Enemy>(e);
	e.set<C_WorldTransform>({pos, 0.f});
	e.set<C_LocalTransform>({pos, 0.f});
	e.set<C_Movement>({});
	e.set<C_Physics>({make_aabb({}, 16, 16)});

	flecs::entity target = world->lookup("player");
	e.set<C_MovementBehavior_FollowTarget>({target, {20, 20}, true});

	e.set<C_Hitbox>({make_circle({8, 8}, 8)});
	e.set<C_Hurtbox>({make_circle({8, 8}, 6)});

	e.set<C_Sprite>({make_sprite("eyeball.ase"), 50});

	return e;
}

flecs::entity make_player(flecs::world *world)
{
	flecs::entity e = world->entity("player");
	e.set<C_Player>({});

	e.set<C_WorldTransform>({});
	e.set<C_LocalTransform>({});
	e.set<C_Input>({45, 45});
	e.set<C_Movement>({});

	e.set<C_Sprite>({make_sprite("character.ase"), 100});

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
	auto &player_scene_node = reg.get<C_WorldTransform>(parent);
	auto &spawner_scene_node = reg.emplace<C_WorldTransform>(e);
	//	player_scene_node.add_child(&spawner_scene_node);

	auto &s = reg.emplace<C_EnemySpawner>(e);
	s.rate = rate;
	s.entity_type = spawn_type;

	return e;
}

flecs::entity make_weapon_boomerang(flecs::world *world, flecs::entity parent)
{
	flecs::entity e = world->entity().child_of(parent);
	e.set<C_WorldTransform>({});
	e.set<C_LocalTransform>({});
	e.set<C_Movement>({});

	C_Weapon w = {};
	w.rate = 0.3f;
	w.weapon_type = WEAPON_BOOMERANG;
	w.targeting_radius = 64.f;
	w.target_type = TARGET_RANDOM_DIR;
	e.set<C_Weapon>(w);

	C_DebugDrawCircle dbg = {};
	dbg.color = make_color(0.0f, 0.5f, 1.0f, 0.5f);
	dbg.circle = make_circle({}, w.targeting_radius);
	e.set<C_DebugDrawCircle>(dbg);

	return e;
}