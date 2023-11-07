#pragma once
#include <cute.h>

struct SpriteComponent : public Cute::Sprite
{
	SpriteComponent(const Cute::Sprite &s) : Cute::Sprite(s)
	{
	}

	SpriteComponent &operator=(const Cute::Sprite &s)
	{
		Cute::Sprite::operator=(s);
		return *this;
	}

	int layer = 0;
};
