#include "sys/lifetime_system.h"
#include "cmp/lifetime_component.h"

void LifetimeSystem::update(World &world)
{
	world.view<LifetimeComponent>().each([&](auto e,
											 LifetimeComponent &lifetime) {
		lifetime -= DELTA_TIME_FIXED;

		if (lifetime <= 0)
		{
			world.destroy(e);
		}
	});
}
