#include "sys/debug_draw_system.h"
#include "cmp/debug_draw_circle.h"
#include "cmp/transform.h"

#include <cute.h>

void debug_draw_system(World &world)
{
	world.view<DebugDrawCircle, Transform>().each(
		[&](auto e, DebugDrawCircle &dbg, Transform &scene_node) {
			Color color = dbg.color;

			Circle circle = dbg.circle;
			circle.p += scene_node.get_global_transform().pos;

			cf_draw_push_color(color);
			cf_draw_circle_fill(circle);
			cf_draw_pop_color();

			color.a = 1.f;
			cf_draw_push_color(color);
			cf_draw_circle(circle, 0.1f);
			cf_draw_pop_color();
		}
	);
}
