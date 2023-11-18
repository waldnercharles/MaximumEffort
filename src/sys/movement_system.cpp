#include "sys/movement_system.h"
#include "cmp/movement.h"
#include "cmp/transform.h"

#include <cute.h>

void movement_system(World &w)
{
	w.view<Movement, Transform>().each([](auto e, Movement &m, Transform &s) {
		auto &t = s.get_local_transform();

		s.set_local_transform(
			{t.pos + m.vel * DELTA_TIME_FIXED,
			 fmodf(t.angle + m.angular_vel * DELTA_TIME_FIXED, PI * 2.f)}
		);
	});
}