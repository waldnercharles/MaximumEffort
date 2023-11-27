#pragma once
#include "common.h"

namespace prefabs
{
	namespace Enemy
	{
		Entity create(World &world, v2 pos, const char *path);
	}
}
