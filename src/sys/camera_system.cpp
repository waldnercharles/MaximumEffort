#include "sys/camera_system.h"
#include "cmp/player_component.h"
#include "cmp/transform_component.h"

#include <cute.h>

CameraSystem::CameraSystem(int width, int height)
	: camera_size(cf_v2(width, height))
{
}

void CameraSystem::update(World &w)
{
	auto player = w.view<PlayerComponent, TransformComponent>().front();

	if (player != ECS_NULL)
	{
		auto player_pos = w.get<TransformComponent>(player)
							  .get_world_transform()
							  .pos;

		cf_camera_dimensions(camera_size.x, camera_size.y);
		cf_camera_look_at(player_pos.x, player_pos.y);
	}
}
