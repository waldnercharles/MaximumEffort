#include "xp_system.h"
#include "cmp/drop_xp_component.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/level_component.h"
#include "cmp/pickup_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "cmp/xp_component.h"
#include "events/death_event.h"
#include "events/level_up_event.h"
#include "events/pickup_collected_event.h"
#include "events/xp_gained_event.h"
#include "log.h"

XpSystem::XpSystem(World &world, EventBus &event_bus) : event_bus(event_bus)
{
	subscriptions.add(event_bus.on<DeathEvent>([&](DeathEvent event) {
		auto *transform = world.try_get<C_Transform>(event.entity);
		auto *xp_drop = world.try_get<C_DropXp>(event.entity);
		if (transform && xp_drop)
		{
			auto pos = transform->get_world_transform().pos;
			auto amount = xp_drop->amount;

			auto e = world.create();
			world.emplace<C_Pickup>(e, 4.f);
			world.emplace<C_XpPickup>(e, amount);
			world.emplace<C_Transform>(e, pos);
			auto &sprite = world.emplace<C_Sprite>(e, "xp.ase");
			sprite.layer = 50;
		}
	}));

	subscriptions.add(
		event_bus.on<PickupCollectedEvent>([&](PickupCollectedEvent event) {
			auto *xp = world.try_get<C_XpPickup>(event.pickup);
			if (xp)
			{
				event_bus.emit<XpGainedEvent>(event.target, xp->amount);
				world.emplace<C_FrameDestroy>(event.pickup);
			}
		})
	);

	subscriptions.add(event_bus.on<XpGainedEvent>([&](XpGainedEvent event) {
		auto *l = world.try_get<C_Level>(event.entity);
		if (l)
		{
			l->xp += event.amount;
			if (l->xp >= l->xp_level_threshold)
			{
				l->level++;
				l->xp_level_threshold = l->xp + l->get_xp_to_next_level();

				event_bus.emit<LevelUpEvent>(event.entity, l->level);
				log_info("Level {}, Current XP {}", l->level, l->xp);
			}
		}
	}));
}

XpSystem::~XpSystem()
{
	for (auto &sub : subscriptions)
	{
		event_bus.off(sub);
	}
}

void XpSystem::update(World &world)
{
}
