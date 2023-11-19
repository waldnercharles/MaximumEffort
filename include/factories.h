#pragma once

#include "cmp/enemy_spawner_component.h"
#include "common.h"

#include <cute.h>
#include <entt/entt.hpp>

Entity make_enemy_eyeball(World &reg, v2 pos, Entity target);

Entity
make_enemy_spawner(World &reg, Entity parent, float rate, EnemyType spawn_type);

Entity make_weapon_boomerang(World &reg, Entity parent);
