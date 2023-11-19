#pragma once
#include "common.h"
#include "system.h"

struct WeaponSystem final : public System
{
	WeaponSystem();
	void update(World &world) override;

private:
	Rnd rnd;
};
