#pragma once

#include "cute.h"

struct Player
{
	Cute::v2 pos;
	Cute::v2 vel;

	Cute::v2 input_dir;

	Cute::v2 speed = Cute::V2(45, 45);

	float facing;

	Cute::Sprite sprite;

	void update(float dt);
	void draw();
};
