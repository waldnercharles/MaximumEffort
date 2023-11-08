#pragma once
#include <cute.h>

struct DebugDrawCircleComponent
{
	Cute::Circle circle = {};
	Cute::Color color = {};
	float thickness = {};
};
