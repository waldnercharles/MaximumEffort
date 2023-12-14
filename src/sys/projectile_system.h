#pragma once
#include "aabb_grid.h"
#include "events/event_bus.h"
#include "system.h"

struct ProjectileSystem final : public System
{
	ProjectileSystem(EventBus &event_bus, AabbGrid<Entity> &enemy_grid);
	void update(World &world) override;

private:
	EventBus &event_bus;
	AabbGrid<Entity> &enemy_grid;
};
