#include "sys/render_system.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "tiled/tiled_map.h"

#include <cute.h>

void RenderSystem::update(World &world)
{
	world.view<TiledMap>().each([](TiledMap &map) {
		map.draw();
	});

	world.view<C_DebugDrawCircle, C_Transform>().each(
		[](auto e, C_DebugDrawCircle &dbg, C_Transform &t) {
			Circle circle = dbg.circle;
			circle.p += t.get_world_transform().pos;

			cf_draw_push_color(dbg.fill);
			cf_draw_circle_fill(circle);
			cf_draw_pop_color();

			if (dbg.thickness > 0)
			{
				cf_draw_push_color(dbg.outline);
				cf_draw_circle(circle, dbg.thickness);
				cf_draw_pop_color();
			}
		}
	);

	//	world.sort<TransformComponent>([](const auto &lhs, const auto &rhs) {
	//		return lhs.get_world_transform().pos.y <
	//			   rhs.get_world_transform().pos.y;
	//	});

	world.view<C_Transform, C_Sprite>().each(
		[](auto e, C_Transform &transform, C_Sprite &sprite) {
			auto world_transform = transform.get_world_transform();

			sprite.transform.p = world_transform.pos;
			sprite.transform.r = cf_sincos_f(world_transform.angle);

			cf_draw_push_layer(sprite.layer);
			sprite.update();
			sprite.draw();
			cf_draw_pop_layer();
		}
	);
}
