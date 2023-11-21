#include "prefabs/enemy_eyeball_prefab.h"
#include "cmp/enemy_component.h"
#include "cmp/health_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

Entity prefabs::EnemyEyeball::create(World &world, v2 pos, Entity target)
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

	auto &health = world.emplace<HealthComponent>(e);
	health.current = 1000;
	health.max = 1000;

	auto &sprite = world.emplace<SpriteComponent>(
		e,
		cf_make_sprite("eyeball.ase")
	);
	sprite.layer = 50;

	return e;
}