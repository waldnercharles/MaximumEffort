#include "sys/camera_system.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "game.h"
#include <cute.h>


using namespace Cute;

void camera_system(entt::registry &reg)
{
	auto view = reg.view<MovementComponent, PlayerComponent>();
	auto e = view.front();

	auto &m = view.get<MovementComponent>(e);

	camera_dimensions(game.camera_size.x, game.camera_size.y);
	camera_look_at(m.pos);
}
