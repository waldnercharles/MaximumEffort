#include "event_handlers/hit_event_handler.h"
#include "cmp/damage_component.h"
#include "cmp/health_component.h"
#include "cmp/hit_component.h"
#include "cmp/hit_immunity_component.h"
#include "cmp/hitbox_component.h"
#include "events/damage_event.h"
#include "events/hit_event.h"

HitEventHandler::HitEventHandler(World &world, EventBus &event_bus)
	: event_bus(event_bus),
	  rnd(cf_rnd_seed(time(nullptr)))
{
	subscription = event_bus.on<HitEvent>([&](HitEvent event) {
		Entity a = event.attacker;
		Entity b = event.defender;

		const auto &hit = world.get<C_Hit>(a);
		auto &hit_immunity = world.get_or_emplace<C_HitImmunity>(b);

		hit_immunity.hits[hit_immunity.count] = {hit.id, hit.frame_immunity};
		hit_immunity.count = cf_min(hit_immunity.count + 1, HIT_IMMUNITY_COUNT);

		auto *attacker_dmg = world.try_get<C_Damage>(a);
		auto *defender_health = world.try_get<C_Health>(b);

		if (attacker_dmg && defender_health)
		{
			auto dmg = cf_rnd_next_range_int(
				&rnd,
				attacker_dmg->min,
				attacker_dmg->max
			);

			defender_health->current -= dmg;
			event_bus.emit<DamageEvent>(event.defender, dmg);
		}
	});
}

HitEventHandler::~HitEventHandler()
{
	event_bus.off(subscription);
}