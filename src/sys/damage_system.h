#pragma once

#include "common.h"
#include "event.h"

struct DamageSystem
{
	DamageSystem(World &, EventBus &);
	~DamageSystem();

private:
	EventBus &event_bus;
	Rnd rnd;

	subscription_id subscription;
};