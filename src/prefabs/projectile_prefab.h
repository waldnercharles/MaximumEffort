#pragma once
#include "common.h"

namespace prefabs
{
	namespace Projectile
	{
		Entity create(World &w, Entity owner, v2 pos, float angle, u16 id);
	}
}
