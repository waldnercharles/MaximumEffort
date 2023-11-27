#pragma once
#include "common.h"
#include "enemy_type.h"

namespace prefabs
{
	namespace EnemySpawner
	{
		Entity create(
			World &,
			EnemyType,
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
