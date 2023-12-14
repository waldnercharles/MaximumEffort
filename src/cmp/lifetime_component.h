#pragma once

struct C_Lifetime
{
	C_Lifetime &operator=(const float &f)
	{
		value = f;
		return *this;
	}

	C_Lifetime &operator-=(const float &f)
	{
		value -= f;
		return *this;
	}

	C_Lifetime &operator+=(const float &f)
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
