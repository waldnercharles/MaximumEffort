#pragma once
#include "common.h"
#include "system.h"

struct AnimationSystem final : public System
{
	void update(World &world) override;
};
