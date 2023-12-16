#pragma once
#include "events/event_bus.h"
#include "system.h"

struct PickupSystem : public System
{
	PickupSystem(EventBus &event_bus);

	void update(World &c_entity) override;

private:
	EventBus event_bus;
};