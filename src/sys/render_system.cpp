#include "sys/render_system.h"
#include "cmp/scene_node_component.h"
#include "cmp/sprite_component.h"

#include <cute.h>

using namespace Cute;

void render_system(entt::registry &reg)
{
	reg.view<SpriteComponent, SceneNodeComponent>().each(
		[&](auto e, SpriteComponent &s, SceneNodeComponent &n) {
			auto t = n.get_global_transform();

			s.transform.p = t.pos;
			s.transform.r = sincos(t.angle);

			draw_push_layer(s.layer);
			s.update();
			s.draw();
			draw_pop_layer();
		}
	);
}