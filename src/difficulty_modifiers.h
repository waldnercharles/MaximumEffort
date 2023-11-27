#pragma once
#include "game_timer.h"

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

struct DifficultySystem
{
	void update()
	{
	}

	float get_health_modifier()
	{
		float modifier = 1 + difficulty.speed_offset;
		modifier += time_progress * difficulty.speed_time_modifier;
	}

	LevelDifficulty difficulty = {};
	GameTimer &game_timer;

private:
	float time_progress;
};