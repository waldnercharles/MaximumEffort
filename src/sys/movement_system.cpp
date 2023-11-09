#include "sys/movement_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_transform.h"

#include <cute.h>

using namespace Cute;

void movement_system(entt::registry &reg, float dt)
{
	reg.view<C_Movement, C_Transform>().each(
		[&](auto e, C_Movement &m, C_Transform &s) {
			auto &t = s.get_local_transform();

			s.set_local_transform(
				{t.pos + m.vel * dt,
				 fmodf(t.angle + m.angular_vel * dt, CF_PI * 2.f)}
			);
		}
	);
}