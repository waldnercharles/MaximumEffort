#include "sys/render_system.h"
#include "cmp/movement_component.h"
#include "cmp/sprite_component.h"

#include <cute.h>

using namespace Cute;

void render_system(entt::registry &reg)
{
	reg.view<SpriteComponent, MovementComponent>().each(
		[&](auto e, SpriteComponent &s, MovementComponent &m) {
			s.transform.p = m.pos;
			s.transform.r = sincos(m.angle);

			draw_push_layer(s.layer);
			s.update();
			s.draw();
			draw_pop_layer();
		}
	);
}