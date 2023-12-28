#pragma once
#include "aabb_grid.h"
#include "common.h"
#include "system.h"

struct InputSystem final : public System
{
	//	InputSystem(AabbGrid<Entity> &enemy_aabb_grid);

	void update(World &world) override;

private:
	//	AabbGrid<Entity> &enemy_aabb_grid;
};
