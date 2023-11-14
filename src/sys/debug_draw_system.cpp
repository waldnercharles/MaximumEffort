#include "sys/debug_draw_system.h"
#include "cmp/c_debug_draw_circle.h"
#include "cmp/c_transform.h"

#include <cute.h>

using namespace Cute;

void debug_draw_system(entt::registry &reg)
{
	reg.view<C_DebugDrawCircle, C_Transform>().each(
		[&](auto e, C_DebugDrawCircle &dbg, C_Transform &scene_node) {
			Color color = dbg.color;

			Circle circle = dbg.circle;
			circle.p += scene_node.get_global_transform().pos;

			draw_push_color(color);
			draw_circle_fill(circle);
			draw_pop_color();

			color.a = 1.f;
			draw_push_color(color);
			draw_circle(circle, 0.1f);
			draw_pop_color();
		}
	);
}
