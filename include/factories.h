#pragma once

#include "cmp/c_enemy_spawner.h"

#include <cute.h>
#include <flecs.h>

flecs::entity make_enemy_eyeball(flecs::world *world, Cute::v2 pos);
flecs::entity make_player(flecs::world *world);

flecs::entity make_enemy_spawner(
	flecs::world *world,
	flecs::entity parent,
	float rate,
	EnemyType spawn_type
);

flecs::entity make_weapon_boomerang(flecs::world *world, flecs::entity parent);
