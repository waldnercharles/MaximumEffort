#include "sys/input_system.h"
#include "cmp/input.h"
#include "cmp/movement.h"
#include "cmp/player.h"

#include <cute.h>

void input_system(World &world, float dt)
{
	world.view<Input, Movement>().each([&world](auto e, Input &i, Movement &m) {
		i.input_dir = {};

		if (cf_key_down(CF_KEY_W) || cf_key_down(CF_KEY_UP))
		{
			i.input_dir.y += 1;
		}
		if (cf_key_down(CF_KEY_A) || cf_key_down(CF_KEY_LEFT))
		{
			i.input_dir.x -= 1;
		}
		if (cf_key_down(CF_KEY_S) || cf_key_down(CF_KEY_DOWN))
		{
			i.input_dir.y -= 1;
		}
		if (cf_key_down(CF_KEY_D) || cf_key_down(CF_KEY_RIGHT))
		{
			i.input_dir.x += 1;
		}

		Player *p = world.try_get<Player>(e);
		if (p)
		{
			p->facing = i.input_dir.x;
		}

		m.vel = cf_safe_norm(i.input_dir) * i.speed;
	});
}