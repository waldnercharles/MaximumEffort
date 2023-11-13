#pragma once

#include <cute.h>
#include <flecs.h>

flecs::entity
make_projectile_boomerang(flecs::world *world, Cute::v2 pos, Cute::v2 dir);
