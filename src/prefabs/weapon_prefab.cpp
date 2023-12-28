#include "prefabs/weapon_prefab.h"
#include "cmp/bullet_emitter_component.h"
#include "cmp/movement_component.h"
#include "cmp/transform_component.h"
#include "log.h"

Entity prefabs::Weapon::create(World &world, Entity parent)
{
	const Entity e = world.create();
	log_info("Created weapon: {}", (u64)e);

	auto &parent_transform = world.get<C_Transform>(parent);
	auto &child_transform = world.emplace<C_Transform>(e);

	parent_transform.add_child(&child_transform);

	auto &wpn = world.emplace<C_BulletEmitter>(e, parent);
	wpn.interval = 1.f;
	wpn.emission_radius = 128.f;

	world.emplace<C_Movement>(e);

	return e;
}
