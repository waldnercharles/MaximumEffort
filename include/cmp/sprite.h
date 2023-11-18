#pragma once
#include <cute.h>

struct Sprite final : public Cute::Sprite
{
	Sprite(const Cute::Sprite &s) : Cute::Sprite(s)
	{
	}

	Sprite &operator=(const Cute::Sprite &s)
	{
		Cute::Sprite::operator=(s);
		return *this;
	}

	int layer = 0;
};
