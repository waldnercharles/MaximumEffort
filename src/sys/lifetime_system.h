#pragma once
#include "common.h"
#include "sys/system.h"

struct LifetimeSystem final : public System
{
	void update(World &world) override;
};