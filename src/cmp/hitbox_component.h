#pragma once
#include <cute.h>

struct HitImmunity
{
	u32 id;
	f32 time_remaining;
};

struct HitboxComponent
{
	Circle circle;
	HitImmunity immune_to[128];
};
