#include "sys/render_system.h"
#include "cmp/sprite.h"
#include "cmp/transform.h"

#include <cute.h>

void render_system(World &w)
{
	w.view<Sprite, Transform>().each([&](auto e, Sprite &s, Transform &n) {
		auto t = n.get_global_transform();

		s.transform.p = t.pos;
		s.transform.r = cf_sincos_f(t.angle);

		cf_draw_push_layer(s.layer);
		s.update();
		s.draw();
		cf_draw_pop_layer();
	});
}