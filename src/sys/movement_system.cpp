#include "sys/movement_system.h"
#include "cmp/movement_component.h"
#include "cmp/scene_node_component.h"

#include <cute.h>

using namespace Cute;

void movement_system(entt::registry &reg, float dt)
{
	reg.view<MovementComponent>().each([&](auto e, MovementComponent &m) {
		m.vel = safe_norm(m.dir) * m.speed * dt;
		m.pos = m.pos + m.vel;

		m.angle = fmodf(m.angle + m.angular_vel * dt, CF_PI * 2.f);

		SceneNodeComponent *scene_node = reg.try_get<SceneNodeComponent>(e);
		if (scene_node)
		{
			SceneNodeTransform transform = scene_node->get_transform();
			transform.pos = m.pos;

			scene_node->set_transform(transform);
		}
	});
}