#pragma once
#include "common.h"
#include "system.h"

struct MovementBehaviorSystem final : public System
{
	void update(World &world) override;
};
