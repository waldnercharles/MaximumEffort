#pragma once
#include "aabb_grid.h"
#include "common.h"
#include "system.h"

struct WeaponSystem final : public System
{
	WeaponSystem(AabbGrid<Entity> &enemy_aabb_grid);
	void update(World &world) override;

private:
	Rnd rnd;
	AabbGrid<Entity> &enemy_aabb_grid;
};
