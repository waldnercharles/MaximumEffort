#pragma once
#include "common.h"

#define HIT_IMMUNITY_COUNT 64

struct HitImmunityComponent
{
	struct HitImmunity
	{
		u16 id = 0;
		u8 remaining_frames = 255;
	};

	HitImmunity hits[HIT_IMMUNITY_COUNT] {};
	int count = 0;
};
