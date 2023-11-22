#pragma once
#include "common.h"
#include "system.h"

struct InputSystem final : public System
{
	void update(World &world) override;
};
