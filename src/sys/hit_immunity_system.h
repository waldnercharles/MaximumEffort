#pragma once
#include "sys/system.h"

struct HitImmunitySystem final : public System
{
	void update(World &world) override;
};