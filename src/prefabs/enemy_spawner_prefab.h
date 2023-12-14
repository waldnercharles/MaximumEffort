#pragma once
#include "common.h"

namespace prefabs
{
	namespace EnemySpawner
	{
		Entity create(
			World &,
			const char *type,
			int start,
			int end,
			float interval,
			int spawns_per_interval,
			int max_spawns,
			bool spawn_once,
			Entity parent
		);

	}
}
