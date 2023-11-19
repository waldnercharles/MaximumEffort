#pragma once
#include "common.h"
#include "system.h"

struct PlayerAnimationSystem final : public System
{
	void update(World &world) override;
};
