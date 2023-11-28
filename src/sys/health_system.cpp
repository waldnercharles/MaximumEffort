#include "health_system.h"
#include "cmp/health_component.h"

void HealthSystem::update(World &world)
{
	world.view<HealthComponent, StatsComponent>().each([](HealthComponent &h,
														  StatsComponent &s) {
		h.current = cf_min(h.current, s.get_stats().health);
	});
}
