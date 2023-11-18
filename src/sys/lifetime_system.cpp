#include "sys/lifetime_system.h"
#include "cmp/lifetime.h"

void lifetime_system(World &world, float dt)
{
	world.view<Lifetime>().each([&](auto e, Lifetime &lifetime) {
		lifetime -= dt;

		if (lifetime <= 0)
		{
			world.destroy(e);
		}
	});
}
