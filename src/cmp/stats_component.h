#pragma once
#include "common.h"
#include "modifiers_component.h"

enum Stat
{
	health,
	speed,
	attack_speed,
	attack_range,
	damage,
	total,
};

struct Stats
{
	inline float &operator[](int i)
	{
		return stat[i];
	}

	inline const float &operator[](int i) const
	{
		return stat[i];
	}

private:
	float stat[Stat::total];
};

struct StatsModifier
{
	inline Modifier &operator[](int i)
	{
		return modifiers[i];
	}

	inline const Modifier &operator[](int i) const
	{
		return modifiers[i];
	}

private:
	Modifier modifiers[Stat::total];
};

inline StatsModifier operator+(const StatsModifier &a, const StatsModifier &b)
{
	StatsModifier result = {};

	for (auto i = 0; i < Stat::total; i++)
	{
		result[i] = a[i] + b[i];
	}

	return result;
}

inline Stats operator*(const Stats &s, const StatsModifier &m)
{
	Stats result = {};

	for (auto i = 0; i < Stat::total; i++)
	{
		result[i] = s[i] * m[i];
	}

	return result;
}

inline Stats operator/(const Stats &s, const StatsModifier &m)
{
	Stats result = {};

	for (auto i = 0; i < Stat::total; i++)
	{
		result[i] = s[i] / m[i];
	}

	return result;
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

	inline const float get(Stat stat) const
	{
		return modified[stat];
	}

	inline void add_modifier(const StatsModifier &modifier)
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
