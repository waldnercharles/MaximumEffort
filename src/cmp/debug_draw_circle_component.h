#pragma once
#include "common.h"
#include <cute.h>

struct DebugDrawCircleComponent
{
	Circle circle = {};
	Color fill = cf_color_invisible();
	Color outline = cf_color_invisible();

	float thickness = 0.1f;
};
