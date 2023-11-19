#pragma once
#include "common.h"
#include <cute.h>

struct MovementBehavior_ConstantDirectionComponent
{
	v2 dir;
	v2 speed;
};