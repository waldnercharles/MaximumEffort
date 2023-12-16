#include "enemy_factory.h"
#include "cmp/age_component.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/drop_xp_component.h"
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
#include "cmp/transform_component.h"
#include "log.h"
#include "serialization/json.h"

Func<Entity, v2> EnemyFactory::load_prototype(const char *path)
{
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

	return [=](v2 pos) {
		Entity e = world.create();
		world.emplace<C_Enemy>(e);
		world.emplace<C_Age>(e);
		world.emplace<C_DropXp>(e, 1);
		auto &t = world.emplace<C_Transform>(e);
		t.set_pos(pos);

		world.emplace<C_Movement>(e);
		world.emplace<C_Facing>(e);

		auto &b = world.emplace<MovementBehavior_FollowTargetComponent>(e);
		b.entity = world.view<C_Player>().front();

		auto &c_stats = world.emplace<C_Stats>(e, stats);
		// TODO: Get the stats modifier?

		auto rnd_scale = scale * cf_rnd_next_range_float(&rnd, 0.9f, 1.1f);

		Circle hitbox = cf_make_circle({}, hitbox_radius * rnd_scale);
		Circle hurtbox = cf_make_circle({}, hurtbox_radius * rnd_scale);

		world.emplace<C_Hitbox>(e, hitbox);
		world.emplace<C_Hurtbox>(e, hurtbox);

		world.emplace<C_Physics>(e, hitbox, mass);

		auto &dbg = world.emplace<C_DebugDrawCircle>(e);
		dbg.circle = hitbox;
		dbg.outline = cf_color_red();

		auto &sprite = world.emplace<C_Sprite>(e, sprite_path);
		sprite.scale = cf_v2(rnd_scale, rnd_scale);
		sprite.layer = 100;

		auto &health = world.emplace<C_Health>(e);
		health.current = c_stats.get_stats().health;

		return e;
	};
}

EnemyFactory::EnemyFactory(World &world, Rnd &rnd) : world(world), rnd(rnd)
{
}

EnemyFactory::EnemyFactory(World &world, Rnd &rnd, const char *dir)
	: EnemyFactory(world, rnd)
{
	load(dir);
}

Entity EnemyFactory::create(const char *name, v2 pos)
{
	auto *fn = prototype_fn.try_get(sintern(name));

	if (fn == nullptr)
	{
		log_error("Unable to find enemy: {}", name);
		return entt::null;
	}

	return (*fn)(pos);
}

void EnemyFactory::load(const char *prototypes_dir)
{
	dir = sintern(prototypes_dir);
	prototype_fn.clear();

	const char **files = cf_fs_enumerate_directory(dir);
	const char **iter;

	for (iter = files; *iter != NULL; iter++)
	{
		char *path = cf_sappend(cf_sset(NULL, dir), *iter);
		const char *filename = sintern(cf_path_get_filename_no_ext(*iter));

		prototype_fn.insert(filename, load_prototype(path));
	}

	cf_fs_free_enumerated_directory(files);
}
