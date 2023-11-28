#pragma once
#include "cmp/stats_component.h"
#include "common.h"
#include "system.h"

struct HealthSystem final : public System
{
	void update(World &world) override;
};
