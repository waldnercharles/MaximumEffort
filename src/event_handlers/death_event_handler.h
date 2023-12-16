#pragma once
#include "events/event_bus.h"
#include "events/subscription.h"

struct DeathEventHandler
{
	DeathEventHandler(World &, EventBus &);
	~DeathEventHandler();

private:
	EventBus &event_bus;
	Rnd rnd;

	Subscription subscription;
};
