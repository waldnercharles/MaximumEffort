#include "sys/debug_draw_system.h"
#include "cmp/debug_draw_circle.h"
#include "cmp/scene_node_component.h"

#include <cute.h>

using namespace Cute;

void debug_draw_system(entt::registry &reg)
{
	reg.view<DebugDrawCircleComponent, SceneNodeComponent>().each(
		[&](auto e,
			DebugDrawCircleComponent &dbg,
			SceneNodeComponent &scene_node) {
			Color color = dbg.color;

			Circle circle = dbg.circle;
			circle.p += scene_node.get_global_transform().pos;

			draw_push_color(color);
			draw_circle_fill(circle);
			draw_pop_color();

			color.a = 1.f;
			draw_push_color(color);
			draw_circle(circle, 0.1);
			draw_pop_color();
		}
	);
}
