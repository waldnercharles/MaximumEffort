#pragma once
#include "cmp/stats_component.h"
#include "common.h"

namespace prefabs
{
	namespace Enemy
	{
		Entity create(World &world, v2 pos, StatsModifier, const char *path);
	}
}
