#include "sys/debug_draw_system.h"
#include "cmp/c_debug_draw_circle.h"
#include "cmp/c_transform.h"

#include <cute.h>

using namespace Cute;

void debug_draw(flecs::world *world)
{
	static flecs::query
		q = world->query<const C_DebugDrawCircle, const C_WorldTransform>();

	q.each([](const C_DebugDrawCircle &dbg, const C_WorldTransform &t) {
		Color color = dbg.color;

		Circle circle = dbg.circle;
		circle.p += t.pos;

		draw_push_color(color);
		draw_circle_fill(circle);
		draw_pop_color();

		color.a = 1.f;
		draw_push_color(color);
		draw_circle(circle, 0.1f);
		draw_pop_color();
	});
}
