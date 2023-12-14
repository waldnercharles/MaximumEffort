#pragma once
#include "common.h"
#include "factories/enemy_factory.h"
#include "system.h"

struct GameTimer;
struct DifficultySystem;

struct SpawnerSystem final : public System
{
	SpawnerSystem(
		Rnd &rnd,
		GameTimer &game_timer,
		DifficultySystem &difficulty_system,
		EnemyFactory &enemy_factory,
		f32 spawn_radius,
		f32 respawn_radius
	);

	void update(World &world) override;

private:
	Rnd &rnd;
	GameTimer &game_timer;
	DifficultySystem &difficulty_system;
	EnemyFactory &enemy_factory;

	f32 spawn_radius;
	f32 respawn_radius_sq;

	v2 get_spawn_offset();
};
