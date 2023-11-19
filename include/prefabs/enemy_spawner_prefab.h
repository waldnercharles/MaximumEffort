#pragma once
#include "common.h"
#include "enemy_type.h"

namespace prefabs
{
	namespace EnemySpawner
	{
		Entity create(World &, Entity parent, float rate, EnemyType enemy_type);

	}
}
