#include "sys/camera_system.h"
#include "cmp/player.h"
#include "cmp/transform.h"
#include "game.h"

#include <cute.h>

void camera_system(World &w)
{
	auto player = w.view<Player, Transform>().front();
	auto player_pos = w.get<Transform>(player).get_global_transform().pos;

	cf_camera_dimensions(game.camera_size.x, game.camera_size.y);
	cf_camera_look_at(player_pos.x, player_pos.y);
}
