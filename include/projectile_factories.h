#pragma once

#include <cute.h>
#include <entt/entt.hpp>

entt::entity
make_projectile_boomerang(entt::registry &reg, Cute::v2 pos, Cute::v2 dir);
