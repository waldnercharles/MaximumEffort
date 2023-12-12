#include "prefabs/player_prefab.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"

Entity prefabs::Player::create(World &world)
{
	const Entity e = world.create();
	world.emplace<TransformComponent>(e);
	world.emplace<PlayerComponent>(e);

	// TODO: Read from file
	Stats stats;
	stats.health = 350.f;
	stats.speed = 90.f;

	world.emplace<StatsComponent>(e, stats);

	world.emplace<FacingComponent>(e);

	world.emplace<InputComponent>(e);
	world.emplace<MovementComponent>(e);

	auto &sprite = world.emplace<SpriteComponent>(e, "character_new.ase");
	sprite.layer = 100;

	return e;
}
