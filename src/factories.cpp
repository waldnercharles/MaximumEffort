#include "factories.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/enemy_component.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "cmp/weapon_component.h"

Entity make_enemy_eyeball(World &world, v2 pos, Entity target)
{
	const Entity e = world.create();
	world.emplace<EnemyComponent>(e);

	auto &enemy_transform = world.emplace<TransformComponent>(e);
	enemy_transform.set_transform({pos});

	world.emplace<MovementComponent>(e);

	auto &
		follow_behavior = world.emplace<MovementBehavior_FollowTargetComponent>(
			e
		);
	follow_behavior.entity = target;
	follow_behavior.speed = {20, 20};
	follow_behavior.face_target = true;

	auto &hitbox = world.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 8);

	auto &hurtbox = world.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 6);

	auto &sprite = world.emplace<SpriteComponent>(
		e,
		cf_make_sprite("eyeball.ase")
	);
	sprite.layer = 50;

	return e;
}

Entity make_enemy_spawner(
	World &world,
	Entity parent,
	float rate,
	EnemyType spawn_type
)
{
	const Entity e = world.create();
	auto &player_transform = world.get<TransformComponent>(parent);
	auto &spawner_transform = world.emplace<TransformComponent>(e);
	player_transform.add_child(&spawner_transform);

	auto &s = world.emplace<EnemySpawnerComponent>(e);
	s.rate = rate;
	s.entity_type = spawn_type;

	return e;
}

Entity make_weapon_boomerang(World &reg, Entity parent)
{
	const Entity e = reg.create();

	auto &parent_transform = reg.get<TransformComponent>(parent);
	auto &child_transform = reg.emplace<TransformComponent>(e);

	parent_transform.add_child(&child_transform);

	auto &w = reg.emplace<WeaponComponent>(e);
	w.rate = 0.3f;
	w.weapon_type = WEAPON_BOOMERANG;
	w.targeting_radius = 64.f;
	w.target_type = TARGET_CLOSEST_ENEMY;

	auto &dbg = reg.emplace<DebugDrawCircleComponent>(e);
	dbg.color = cf_make_color_rgba_f(0, 0.5f, 1, 0.5f);
	dbg.circle = cf_make_circle({0, 0}, w.targeting_radius);

	reg.emplace<MovementComponent>(e);

	return e;
}