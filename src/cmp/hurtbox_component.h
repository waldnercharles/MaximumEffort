#pragma once
#include <cute.h>

struct HurtboxComponent
{
	u32 id = 0;
	float cooldown = 0;

	Circle circle;
};
