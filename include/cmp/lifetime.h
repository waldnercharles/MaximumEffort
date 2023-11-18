#pragma once

struct Lifetime
{
	Lifetime &operator=(const float &f)
	{
		value = f;
		return *this;
	}

	Lifetime &operator-=(const float &f)
	{
		value -= f;
		return *this;
	}

	Lifetime &operator+=(const float &f)
	{
		value += f;
		return *this;
	}

	operator float() const
	{
		return value;
	}

	float value;
};
