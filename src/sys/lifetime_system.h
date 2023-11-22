#pragma once
#include "common.h"
#include "system.h"

struct LifetimeSystem final : public System
{
	void update(World &world) override;
};