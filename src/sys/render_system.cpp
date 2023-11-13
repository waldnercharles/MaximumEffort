#include "sys/render_system.h"
#include "cmp/c_sprite.h"
#include "cmp/c_transform.h"

#include <cute.h>

using namespace Cute;

void render_system(flecs::world *world)
{
	static flecs::query q = world->query<const C_WorldTransform, C_Sprite>();

	q.each([](const C_WorldTransform &t, C_Sprite &s) {
		s.sprite.transform.p = t.pos;
		s.sprite.transform.r = sincos(t.angle);

		draw_push_layer(s.layer);
		s.sprite.update();
		s.sprite.draw();
		draw_pop_layer();
	});
}

//void render_system(entt::registry &reg)
//{
//	reg.view<C_Sprite, C_Transform>().each(
//		[&](auto e, C_Sprite &s, C_Transform &n) {
//			auto t = n.get_global_transform();
//
//			s.transform.p = t.pos;
//			s.transform.r = sincos(t.angle);
//
//			draw_push_layer(s.layer);
//			s.update();
//			s.draw();
//			draw_pop_layer();
//		}
//	);
//}