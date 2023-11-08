#include "sys/render_system.h"
#include "cmp/scene_node_component.h"
#include "cmp/sprite_component.h"
#include "sys/debug_draw_system.h"

#include <cute.h>

using namespace Cute;

void render_system(entt::registry &reg)
{
	reg.view<SpriteComponent, SceneNodeComponent>().each(
		[&](auto e, SpriteComponent &s, SceneNodeComponent &n) {
			s.transform.p = n.get_global_transform().pos;
			//			s.transform.r = sincos(m.angle);

			draw_push_layer(s.layer);
			s.update();
			s.draw();
			draw_pop_layer();
		}
	);

	debug_draw_system(reg);
}