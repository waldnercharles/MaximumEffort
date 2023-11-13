#include "camera.h"
#include "cmp/c_transform.h"
#include "cute.h"
#include "flecs.h"
#include "game.h"

using namespace Cute;

void update_camera(flecs::world *world)
{
	// TODO: Multiplayer split-screen?
	auto e = world->lookup("player");
	const C_WorldTransform *t = e.get<const C_WorldTransform>();
	camera_dimensions(game.camera_size.x, game.camera_size.y);
	camera_look_at(t->pos);
}