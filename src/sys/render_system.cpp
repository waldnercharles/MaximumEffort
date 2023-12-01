#include "sys/render_system.h"
#include "cmp/debug_draw_circle_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"
#include "tiled_map.h"

#include <cute.h>

void RenderSystem::update(World &world)
{
	world.view<TiledMap>().each([](TiledMap &map) {
		map.draw();
	});

	world.view<DebugDrawCircleComponent, TransformComponent>().each(
		[](auto e, DebugDrawCircleComponent &dbg, TransformComponent &t) {
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

	world.view<SpriteComponent, TransformComponent>().each(
		[](auto e, SpriteComponent &sprite, TransformComponent &transform) {
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
