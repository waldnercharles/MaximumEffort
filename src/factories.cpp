#include "factories.h"
#include "cmp/debug_draw_circle.h"
#include "cmp/enemy.h"
#include "cmp/enemy_spawner.h"
#include "cmp/hitbox.h"
#include "cmp/hurtbox.h"
#include "cmp/input.h"
#include "cmp/movement.h"
#include "cmp/movement_behavior_follow_target.h"
#include "cmp/player.h"
#include "cmp/sprite.h"
#include "cmp/transform.h"
#include "cmp/weapon.h"

Entity make_enemy_eyeball(World &world, v2 pos, Entity target)
{
	const Entity e = world.create();
	world.emplace<Enemy>(e);

	auto &enemy_transform = world.emplace<Transform>(e);
	enemy_transform.set_local_transform({pos});

	world.emplace<Movement>(e);

	auto &follow_behavior = world.emplace<MovementBehavior_FollowTarget>(e);
	follow_behavior.entity = target;
	follow_behavior.speed = {20, 20};
	follow_behavior.face_target = true;

	auto &hitbox = world.emplace<Hitbox>(e);
	hitbox.circle = cf_make_circle({0, 0}, 8);

	auto &hurtbox = world.emplace<Hurtbox>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 6);

	auto &sprite = world.emplace<Sprite>(e, cf_make_sprite("eyeball.ase"));
	sprite.layer = 50;

	return e;
}

Entity make_player(World &world)
{
	const Entity e = world.create();
	world.emplace<Transform>(e);
	world.emplace<Player>(e);

	auto &i = world.emplace<Input>(e);
	i.speed = {45, 45};

	world.emplace<Movement>(e);

	auto &s = world.emplace<Sprite>(e, cf_make_sprite("character.ase"));
	s.layer = 100;

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
	auto &player_transform = world.get<Transform>(parent);
	auto &spawner_transform = world.emplace<Transform>(e);
	player_transform.add_child(&spawner_transform);

	auto &s = world.emplace<EnemySpawner>(e);
	s.rate = rate;
	s.entity_type = spawn_type;

	return e;
}

Entity make_weapon_boomerang(World &reg, Entity parent)
{
	const Entity e = reg.create();

	auto &parent_transform = reg.get<Transform>(parent);
	auto &child_transform = reg.emplace<Transform>(e);

	parent_transform.add_child(&child_transform);

	auto &w = reg.emplace<Weapon>(e);
	w.rate = 0.3f;
	w.weapon_type = WEAPON_BOOMERANG;
	w.targeting_radius = 64.f;
	w.target_type = TARGET_CLOSEST_ENEMY;

	auto &dbg = reg.emplace<DebugDrawCircle>(e);
	dbg.color = cf_make_color_rgba_f(0, 0.5f, 1, 0.5f);
	dbg.circle = cf_make_circle({0, 0}, w.targeting_radius);

	reg.emplace<Movement>(e);

	return e;
}