#include "pickup_system.h"
#include "cmp/homing_mover_component.h"
#include "cmp/movement_component.h"
#include "cmp/pickup_collector_component.h"
#include "cmp/pickup_component.h"
#include "cmp/transform_component.h"
#include "events/event_bus.h"
#include "events/pickup_collected_event.h"

PickupSystem::PickupSystem(EventBus &event_bus) : event_bus(event_bus)
{
}

void PickupSystem::update(World &world)
{
	// TODO: Move to the closest pickup collector
	world.view<C_Transform, C_PickupCollector>().each(
		[&](auto c_entity, C_Transform &c_transform, C_PickupCollector &c) {
			auto pos = c_transform.get_world_transform().pos;
			auto pickup_radius_sq = c.pickup_radius * c.pickup_radius;

			world.view<C_Pickup, C_Transform>(entt::exclude<C_HomingMover>)
				.each([&](auto p_entity,
						  C_Pickup &p,
						  C_Transform &pickup_transform) {
					auto d = pos - pickup_transform.get_world_transform().pos;
					if (cf_dot(d, d) <= pickup_radius_sq)
					{
						auto &m = world.emplace<C_Movement>(p_entity);
						m.angular_vel = 2 * PI;

						world.emplace<C_HomingMover>(p_entity, c_entity, 250.f);
					}
				});
		}
	);

	world.view<C_Pickup, C_Transform, C_HomingMover>().each(
		[&](auto e, C_Pickup &p, C_Transform &t, C_HomingMover &h) {
			auto target_pos =
				world.get<C_Transform>(h.target).get_world_transform().pos;
			auto pos = t.get_world_transform().pos;

			auto d = target_pos - pos;
			auto collect_radius_sq = p.collect_radius * p.collect_radius;

			if (cf_dot(d, d) <= collect_radius_sq)
			{
				event_bus.emit<PickupCollectedEvent>(h.target, e);
			}
		}
	);
}
