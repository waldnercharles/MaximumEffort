#include "sys/camera_system.h"
#include "cmp/c_movement.h"
#include "cmp/c_player.h"
#include "cmp/c_transform.h"
#include "game.h"
#include <cute.h>


using namespace Cute;

void camera_system(entt::registry &reg)
{
	auto view = reg.view<C_Player, C_Transform>();
	auto e = view.front();
	auto &scene_node = view.get<C_Transform>(e);

	camera_dimensions(game.camera_size.x, game.camera_size.y);
	camera_look_at(scene_node.get_global_transform().pos);
}
