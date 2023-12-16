#include "prefabs/player_prefab.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/level_component.h"
#include "cmp/movement_component.h"
#include "cmp/pickup_collector_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/stats_component.h"
#include "cmp/transform_component.h"

Entity prefabs::Player::create(World &world)
{
	const Entity e = world.create();
	world.emplace<C_Transform>(e);
	world.emplace<C_Player>(e);

	// TODO: Read from file
	Stats stats;
	stats.health = 350.f;
	stats.speed = 90.f;
	stats.projectile_amount = 1;

	world.emplace<C_Stats>(e, stats);
	world.emplace<C_Level>(e);

	world.emplace<C_Facing>(e);

	world.emplace<C_Input>(e);
	world.emplace<C_Movement>(e);

	auto &sprite = world.emplace<C_Sprite>(e, "character_new.ase");
	sprite.layer = 100;

	world.emplace<C_PickupCollector>(e, 64.f);

	return e;
}
