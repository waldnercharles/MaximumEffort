#include "prefabs/weapon_boomerang_prefab.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"
#include "cmp/weapon_component.h"

Entity prefabs::WeaponBoomerang::create(World &world, Entity parent)
{
	const Entity e = world.create();

	auto &parent_transform = world.get<TransformComponent>(parent);
	auto &child_transform = world.emplace<TransformComponent>(e);

	parent_transform.add_child(&child_transform);

	auto &wpn = world.emplace<WeaponComponent>(e);
	wpn.rate = 0.15f;
	wpn.weapon_type = WEAPON_BOOMERANG;
	wpn.targeting_radius = 64.f;
	wpn.target_type = TARGET_CLOSEST_ENEMY;

	auto &dbg = world.emplace<DebugDrawCircleComponent>(e);
	dbg.color = cf_make_color_rgba_f(0, 0.5f, 1, 0.5f);
	dbg.circle = cf_make_circle({0, 0}, wpn.targeting_radius);

	world.emplace<MovementComponent>(e);

	return e;
}
