#include "sys/lifetime_system.h"
#include "cmp/c_lifetime.h"

void lifetime_system(entt::registry &reg, float dt)
{
	reg.view<C_Lifetime>().each([&](auto e, C_Lifetime &lifetime) {
		lifetime -= dt;

		if (lifetime <= 0)
		{
			reg.destroy(e);
		}
	});
}
