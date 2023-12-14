#include "health_system.h"
#include "cmp/health_component.h"

void HealthSystem::update(World &world)
{
	world.view<C_Health, C_Stats>().each([](C_Health &h, C_Stats &s) {
		h.current = cf_min(h.current, s.get_stats().health);
	});
}
