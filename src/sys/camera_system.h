#pragma once
#include "common.h"
#include "system.h"

struct CameraSystem final : public System
{
	CameraSystem(int width, int height);

	void update(World &) override;

private:
	v2 camera_size;
};
