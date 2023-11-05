#pragma once
#include "player.h"
#include <cute.h>

struct Enemy
{
	Cute::v2 pos;
	Cute::v2 speed;

	Cute::Sprite sprite;

	void update(float dt);
	void draw();
};