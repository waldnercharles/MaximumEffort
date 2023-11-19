#include "prefabs/enemy_spawner_prefab.h"
#include "cmp/enemy_spawner_component.h"
#include "cmp/transform_component.h"

Entity prefabs::EnemySpawner::create(
	World &world,
	Entity parent,
	float rate,
	EnemyType enemy_type
)
{
	const Entity e = world.create();
	auto &player_transform = world.get<TransformComponent>(parent);
	auto &spawner_transform = world.emplace<TransformComponent>(e);
	player_transform.add_child(&spawner_transform);

	auto &s = world.emplace<EnemySpawnerComponent>(e);
	s.rate = rate;
	s.entity_type = enemy_type;

	return e;
}