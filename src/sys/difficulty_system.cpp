#include "difficulty_system.h"
#include "game_timer.h"

DifficultySystem::DifficultySystem(
	LevelDifficulty difficulty,
	GameTimer &game_timer
)
	: difficulty(difficulty),
	  game_timer(game_timer)
{
}

void DifficultySystem::update(World &world)
{
	time_progress = game_timer.get_elapsed() / game_timer.get_initial_time();
}

StatsModifier DifficultySystem::get_stats_modifier() const
{
	StatsModifier stats_modifier = {};
	stats_modifier.health = get_health_modifier();
	stats_modifier.speed = get_speed_modifier();

	return stats_modifier;
}

Modifier DifficultySystem::get_health_modifier() const
{
	Modifier modifier = {};
	modifier.flat = difficulty.health_offset;
	modifier.increase = difficulty.health_time_modifier * time_progress;
	modifier.more = 0.f;

	return modifier;
}

Modifier DifficultySystem::get_speed_modifier() const
{
	Modifier modifier = {};
	modifier.flat = difficulty.speed_offset;
	modifier.increase = difficulty.speed_time_modifier * time_progress;
	modifier.more = 0.f;

	return modifier;
}
