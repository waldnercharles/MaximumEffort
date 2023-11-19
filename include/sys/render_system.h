#pragma once
#include "common.h"
#include "system.h"

struct RenderSystem final : public System
{
	void update(World &world) override;
};
