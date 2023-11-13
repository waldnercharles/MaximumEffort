#pragma once
#include <cute.h>

struct C_Sprite
{
	Cute::Sprite sprite = Cute::Sprite();
	int layer = 0;

	C_Sprite &operator=(const Cute::Sprite &s)
	{
		sprite = s;
		return *this;
	}

	operator Cute::Sprite &()
	{
		return sprite;
	}

	operator Cute::Sprite() const
	{
		return sprite;
	}
};
