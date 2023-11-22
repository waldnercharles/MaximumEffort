#pragma once
#include "common.h"

struct System
{
	virtual void update(World &) = 0;
};
