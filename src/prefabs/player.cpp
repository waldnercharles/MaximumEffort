#include "cmp/input_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "prefabs/player_prefab.h"

Entity prefabs::Player::create(World &world)
{
	const Entity e = world.create();
	world.emplace<TransformComponent>(e);
	world.emplace<PlayerComponent>(e);

	auto &i = world.emplace<InputComponent>(e);
	i.speed = cf_v2(45, 45);

	world.emplace<MovementComponent>(e);

	auto &s = world.emplace<SpriteComponent>(
		e,
		cf_make_sprite("character.ase")
	);
	s.layer = 100;

	return e;
}
