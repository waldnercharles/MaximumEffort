#pragma once
#include "common.h"
#include "events/event_bus.h"
#include "events/subscription.h"
#include "system.h"

struct XpSystem : public System
{
	XpSystem(World &world, EventBus &event_bus);
	~XpSystem();

	void update(World &world) override;

private:
	EventBus &event_bus;
	Array<Subscription> subscriptions;
};