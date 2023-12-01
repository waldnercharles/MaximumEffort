#include "prefabs/enemy_prefab.h"
#include "cmp/age_component.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/enemy_component.h"
#include "cmp/facing_component.h"
#include "cmp/health_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

Entity prefabs::Enemy::create(
	World &world,
	v2 pos,
	StatsModifier stats_modifier,
	const char *path,
	float scale
)
{
	const Entity player = world.view<PlayerComponent>().front();

	const Entity e = world.create();
	world.emplace<EnemyComponent>(e);

	world.emplace<AgeComponent>(e);

	// TODO: Read from file
	Stats stats;
	stats.health = 100;
	stats.speed = 10;

	auto &stats_component = world.emplace<StatsComponent>(e, stats);
	stats_component.add_modifier(stats_modifier);

	world.emplace<FacingComponent>(e);

	auto &enemy_transform = world.emplace<TransformComponent>(e);
	enemy_transform.set_transform({pos, 0.f});

	world.emplace<MovementComponent>(e);

	auto &follow_behavior =
		world.emplace<MovementBehavior_FollowTargetComponent>(e);
	follow_behavior.entity = player;
	follow_behavior.face_target = false;

	auto &hitbox = world.emplace<HitboxComponent>(e);
	hitbox.circle = cf_make_circle({0, 0}, 8);

	auto &debug = world.emplace<DebugDrawCircleComponent>(e);
	debug.circle = hitbox.circle;
	debug.outline = cf_color_red();
	debug.thickness = 0.25f;

	auto &hurtbox = world.emplace<HurtboxComponent>(e);
	hurtbox.circle = cf_make_circle({0, 0}, 6);

	auto &health = world.emplace<HealthComponent>(e);
	health.current = stats_component.get_stats().health;

	auto &sprite = world.emplace<SpriteComponent>(e, cf_make_sprite(path));
	sprite.layer = 50;
	sprite.scale = cf_v2(scale, scale);

	return e;
}