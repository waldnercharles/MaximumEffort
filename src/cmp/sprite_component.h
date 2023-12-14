#pragma once
#include <cute.h>

struct C_Sprite final : public Cute::Sprite
{
	C_Sprite(const char *path) : Sprite(cf_make_sprite(path))
	{
	}

	C_Sprite(const Cute::Sprite &s) : Cute::Sprite(s)
	{
	}

	C_Sprite &operator=(const Cute::Sprite &s)
	{
		Cute::Sprite::operator=(s);
		return *this;
	}

	int layer = 0;
};
