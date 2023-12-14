#include "sys/lifetime_system.h"
#include "cmp/lifetime_component.h"

void LifetimeSystem::update(World &world)
{
	world.view<C_Lifetime>().each([&](auto e, C_Lifetime &lifetime) {
		lifetime -= DELTA_TIME_FIXED;

		if (lifetime <= 0)
		{
			world.destroy(e);
		}
	});
}
