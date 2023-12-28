#pragma once
#include "events/event_bus.h"
#include "events/subscription.h"

struct LevelUpEventHandler
{
	LevelUpEventHandler(EventBus &event_bus);
	~LevelUpEventHandler();

private:
	EventBus &event_bus;
	Subscription subscription;
};