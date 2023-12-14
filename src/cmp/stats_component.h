#pragma once
#include "common.h"

struct Stats
{
	float health;
	float speed;
};

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

inline float operator*(const float &val, const Modifier &m)
{
	return ((val + m.flat) * (m.increase + 1.f)) * (m.more + 1.f);
}

inline float operator/(const float &val, const Modifier &m)
{
	return ((val / (m.more + 1.f)) / (m.increase + 1.f)) - m.flat;
}

struct StatsModifier
{
	Modifier health;
	Modifier speed;
};

inline StatsModifier operator+(const StatsModifier &a, const StatsModifier &b)
{
	return {a.health + b.health, a.speed + b.speed};
}

inline Stats operator*(const Stats &s, const StatsModifier &m)
{
	return {s.health * m.health, s.speed * m.speed};
}

inline Stats operator/(const Stats &s, const StatsModifier &m)
{
	return {s.health / m.health, s.speed / m.speed};
}

struct C_Stats
{
	inline C_Stats(Stats stats) : base(stats), modified(stats)
	{
	}

	inline Stats get_base_stats() const
	{
		return base;
	}

	inline Stats get_stats() const
	{
		return modified;
	}

	inline void add_modifier(StatsModifier &modifier)
	{
		modified = modified * modifier;
		modifiers.add(modifier);
	}

	inline void remove_modifier(int i)
	{
		auto modifier = modifiers[i];
		modified = modified / modifier;
		modifiers.unordered_remove(i);
	}

private:
	Stats base;
	Stats modified;

	Array<StatsModifier> modifiers;
};
