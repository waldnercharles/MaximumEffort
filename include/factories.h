#pragma once

#include "cmp/c_enemy_spawner.h"

#include <cute.h>
#include <entt/entt.hpp>
#include <flecs.h>

flecs::entity make_enemy_eyeball(flecs::world *world, Cute::v2 pos);
flecs::entity make_player(flecs::world *world);

entt::entity make_enemy_spawner(
	entt::registry &reg,
	entt::entity parent,
	float rate,
	EnemyType spawn_type
);

flecs::entity make_weapon_boomerang(flecs::world *world, flecs::entity parent);
