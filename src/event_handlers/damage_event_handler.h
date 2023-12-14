#pragma once

#include "common.h"
#include "events/event_bus.h"

struct DamageEventHandler
{
	DamageEventHandler(World &, EventBus &);
	~DamageEventHandler();

private:
	EventBus &event_bus;
	Rnd rnd;

	Subscription subscription;
};