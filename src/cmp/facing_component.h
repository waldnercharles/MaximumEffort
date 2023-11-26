#pragma once
#include "common.h"

enum Facing : u8
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct FacingComponent
{
	Facing facing;
};
