#include "enemies.h"
#include "cmp/age_component.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/enemy_component.h"
#include "cmp/facing_component.h"
#include "cmp/health_component.h"
#include "cmp/hitbox_component.h"
#include "cmp/hurtbox_component.h"
#include "cmp/movement_behavior_follow_target_component.h"
#include "cmp/movement_component.h"
#include "cmp/physics_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"
#include "json.h"
#include "log.h"

Func<Entity, World &, Rnd &, v2> load_enemy_prototype(const char *path)
{
	EnemeyPrototypeMap prototypes;

	auto doc = cf_make_json_from_file(path);
	auto obj = cf_json_get_root(doc);
	if (!cf_json_is_object(obj))
	{
		log_info("Skipping invalid enemy at {}.", path);
		return nullptr;
	}

	Stats stats;
	{
		auto stats_json = cf_json_get(obj, "stats");
		stats.health = cf_json_get_float(stats_json, "health");
		stats.speed = cf_json_get_float(stats_json, "speed");
	}

	float scale = cf_json_get_float(obj, "scale");
	float hitbox_radius = cf_json_get_float(obj, "hitbox");
	float hurtbox_radius = cf_json_get_float(obj, "hurtbox");
	const char *sprite_path = sintern(cf_json_get_string(obj, "sprite"));

	float mass = cf_json_get_float(obj, "mass");
	mass = mass == 0 ? 1.f : mass;

	return [=](World &world, Rnd &rnd, v2 pos) {
		Entity e = world.create();
		world.emplace<EnemyComponent>(e);
		world.emplace<AgeComponent>(e);
		auto &t = world.emplace<TransformComponent>(e);
		t.set_pos(pos);

		world.emplace<MovementComponent>(e);
		world.emplace<FacingComponent>(e);

		auto &b = world.emplace<MovementBehavior_FollowTargetComponent>(e);
		b.entity = world.view<PlayerComponent>().front();

		auto &c_stats = world.emplace<StatsComponent>(e, stats);
		// TODO: Get the stats modifier?

		auto rnd_scale = scale * cf_rnd_next_range_float(&rnd, 0.9f, 1.1f);

		Circle hitbox = cf_make_circle({}, hitbox_radius * rnd_scale);
		Circle hurtbox = cf_make_circle({}, hurtbox_radius * rnd_scale);

		world.emplace<HitboxComponent>(e, hitbox);
		world.emplace<HurtboxComponent>(e, hurtbox);

		world.emplace<PhysicsComponent>(e, hitbox, mass);

		auto &dbg = world.emplace<DebugDrawCircleComponent>(e);
		dbg.circle = hitbox;
		dbg.outline = cf_color_red();

		Sprite sprite = cf_make_sprite(sprite_path);
		sprite.scale = cf_v2(rnd_scale, rnd_scale);

		world.emplace<SpriteComponent>(e, sprite);

		auto &health = world.emplace<HealthComponent>(e);
		health.current = c_stats.get_stats().health;

		return e;
	};
}
