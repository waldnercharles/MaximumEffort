#include "sys/movement_system.h"
#include "cmp/movement_component.h"

#include <cute.h>

using namespace Cute;

void movement_system(entt::registry &reg, float dt)
{
	reg.view<MovementComponent>().each([&](auto e, MovementComponent &m) {
		m.vel = m.dir * m.speed * dt;
		m.pos = m.pos + m.vel;

		m.angle = fmodf(m.angle + m.angular_vel * dt, CF_PI * 2.f);
	});
}