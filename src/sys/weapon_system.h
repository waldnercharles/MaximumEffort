#pragma once
#include "aabb_grid.h"
#include "common.h"
#include "system.h"

struct GameTimer;

struct WeaponSystem final : public System
{
	WeaponSystem(AabbGrid<Entity> &enemy_aabb_grid, GameTimer &game_timer);
	void update(World &world) override;

private:
	Rnd rnd;
	AabbGrid<Entity> &enemy_aabb_grid;
	GameTimer &game_timer;

	u16 next_attack_id;
};
