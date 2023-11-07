#pragma once

struct LifetimeComponent
{
	LifetimeComponent &operator=(const float &f)
	{
		value = f;
		return *this;
	}

	LifetimeComponent &operator-=(const float &f)
	{
		value -= f;
		return *this;
	}

	LifetimeComponent &operator+=(const float &f)
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
