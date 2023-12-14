#include "prefabs/enemy_spawner_prefab.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/transform_component.h"

Entity prefabs::EnemySpawner::create(
	World &world,
	const char *type,
	int start,
	int end,
	float interval,
	int spawns_per_interval,
	int max_spawns,
	bool spawn_once,
	Entity parent
)
{
	const Entity e = world.create();

	auto &parent_transform = world.get<C_Transform>(parent);
	auto &spawner_transform = world.emplace<C_Transform>(e);
	parent_transform.add_child(&spawner_transform);

	auto &s = world.emplace<C_EnemySpawner>(e);
	s.start = start;
	s.end = end;
	s.enemy_type = type;
	s.interval = interval;
	s.spawns_per_interval = spawns_per_interval;
	s.max_spawns = max_spawns;
	s.spawn_once = spawn_once;

	return e;
}