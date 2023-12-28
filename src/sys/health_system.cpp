#include "health_system.h"
#include "cmp/health_component.h"
#include "events/death_event.h"

HealthSystem::HealthSystem(EventBus &event_bus) : event_bus(event_bus)
{
}

void HealthSystem::update(World &world)
{
	world.view<C_Health, C_Stats>().each(
		[this](auto e, C_Health &h, C_Stats &s) {
			h.current = cf_min(h.current, s.get(Stat::health));

			if (h.current <= 0)
			{
				event_bus.emit<DeathEvent>({e});
			}
		}
	);
}
