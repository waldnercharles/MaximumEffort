#include "prefabs/player_prefab.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

Entity prefabs::Player::create(World &world)
{
	const Entity e = world.create();
	world.emplace<TransformComponent>(e);
	world.emplace<PlayerComponent>(e);
	auto &f = world.emplace<FacingComponent>(e);
	f.facing = Facing::NONE;

	auto &i = world.emplace<InputComponent>(e);
	i.speed = cf_v2(45, 45);

	world.emplace<MovementComponent>(e);

	auto &s = world.emplace<SpriteComponent>(e, "character.ase");
	s.layer = 100;

	return e;
}
