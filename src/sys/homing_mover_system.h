#pragma once
#include "system.h"

struct HomingMoverSystem : public System
{
	void update(World &world) override;
};