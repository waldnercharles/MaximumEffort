#pragma once

#include "cmp/spawner_component.h"

#include <cute.h>
#include <entt/entt.hpp>

entt::entity make_enemy_eyeball(entt::registry &reg, Cute::v2);
entt::entity make_player(entt::registry &reg);

entt::entity make_enemy_spawner(
	entt::registry &reg,
	entt::entity parent,
	float rate,
	EnemyType spawn_type
);

entt::entity make_weapon_boomerang(entt::registry &reg, entt::entity parent);
