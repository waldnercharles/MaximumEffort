#pragma once

#include "common.h"
#include "events/event_bus.h"

struct HitEventHandler
{
	HitEventHandler(World &, EventBus &);
	~HitEventHandler();

private:
	EventBus &event_bus;
	Rnd rnd;

	Subscription subscription;
};