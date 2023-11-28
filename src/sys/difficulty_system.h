#pragma once
#include "cmp/stats_component.h"
#include "system.h"

struct LevelDifficulty
{
	float health_offset;
	float health_time_modifier;

	float speed_offset;
	float speed_time_modifier;

	float enemy_damage_offset;
	float enemy_damage_time_modifier;

	float armor_offset;
	float armor_time_modifier;

	float spawn_interval_offset;
	float spawn_interval_time_modifier;

	float spawn_max_offset;
	float spawn_max_time_modifier;

	float gold_offset;
	float gold_time_modifier;
};

struct GameTimer;

struct DifficultySystem final : public System
{
	DifficultySystem(LevelDifficulty difficulty, GameTimer &game_timer);

	void update(World &world) override;

	StatsModifier get_stats_modifier() const;

	LevelDifficulty difficulty = {};

private:
	float time_progress;

	GameTimer &game_timer;

	Modifier get_health_modifier() const;
	Modifier get_speed_modifier() const;
};