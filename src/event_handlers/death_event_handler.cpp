#include "death_event_handler.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"
#include "events/death_event.h"

DeathEventHandler::DeathEventHandler(World &world, EventBus &event_bus)
	: event_bus(event_bus)
{
	subscription = event_bus.on<DeathEvent>([&world](DeathEvent event) {
		auto e = event.entity;
		if (world.all_of<C_Player>(e))
		{
			// TODO: Something special?
		}
		else
		{
			world.get_or_emplace<C_FrameDestroy>(e);
		}
	});
}

DeathEventHandler::~DeathEventHandler()
{
	event_bus.off(subscription);
}
