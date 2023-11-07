#include "sys/target_system.h"
#include "cmp/movement_component.h"
#include "cmp/target_component.h"

using namespace Cute;

void target_system(entt::registry &reg, float dt)
{
	reg.view<MovementComponent, TargetComponent>().each(
		[&](auto e, MovementComponent &m, TargetComponent &t) {
			auto *other = reg.try_get<MovementComponent>(t.entity);

			if (other)
			{
				m.dir = safe_norm(other->pos - m.pos);

				if (t.face_target)
				{
					m.angle = atan2(m.dir.y, m.dir.x);
				}
			}
		}
	);
}
