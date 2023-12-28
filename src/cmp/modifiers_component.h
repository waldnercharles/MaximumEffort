#pragma once
#include "common.h"

struct Modifier
{
	float flat = 0.f;
	float increase = 0.f;
	float more = 0.f;
};

inline Modifier operator+(const Modifier &a, const Modifier &b)
{
	return {
		a.flat + b.flat,
		a.increase + b.increase,
		((a.more + 1.f) * (b.more + 1.f)) - 1.f};
}

inline Modifier &operator+=(Modifier &a, const Modifier &b)
{
	return a = a + b;
}

inline float operator*(const float &val, const Modifier &m)
{
	return ((val + m.flat) * (m.increase + 1.f)) * (m.more + 1.f);
}

inline float operator/(const float &val, const Modifier &m)
{
	return ((val / (m.more + 1.f)) / (m.increase + 1.f)) - m.flat;
}

struct C_Modifiers
{
	Map<const char *, Modifier> modifiers;
};