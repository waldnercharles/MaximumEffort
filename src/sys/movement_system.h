#pragma once
#include "common.h"
#include "system.h"

struct MovementSystem final : public System
{
	void update(World &world) override;
};
