#pragma once
#include "cmp/stats_component.h"
#include "common.h"
#include "events/event_bus.h"
#include "system.h"

struct HealthSystem final : public System
{
	HealthSystem(EventBus &event_bus);

	void update(World &world) override;

private:
	EventBus &event_bus;
};
