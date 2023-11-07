#include "sys/lifetime_system.h"
#include "cmp/lifetime_component.h"

void lifetime_system(entt::registry &reg, float dt)
{
	reg.view<LifetimeComponent>().each([&](auto e,
										   LifetimeComponent &lifetime) {
		lifetime -= dt;

		if (lifetime <= 0)
		{
			reg.destroy(e);
		}
	});
}
