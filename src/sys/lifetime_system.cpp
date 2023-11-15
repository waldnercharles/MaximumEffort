#include "sys/lifetime_system.h"
#include "cmp/c_lifetime.h"

void add_lifetime_system(flecs::world *world)
{
	world->system<C_Lifetime>("lifetime")
		.each([](flecs::iter &it, size_t i, C_Lifetime &lifetime) {
			lifetime -= it.delta_time();

			if (lifetime <= 0)
			{
				it.entity(i).destruct();
			}
		});
}
