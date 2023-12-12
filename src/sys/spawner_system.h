#pragma once
#include "common.h"
#include "prototype_map.h"
#include "system.h"

struct GameTimer;
struct DifficultySystem;

struct SpawnerSystem final : public System
{
	SpawnerSystem(
		f32 spawn_radius,
		f32 respawn_radius,
		const GameTimer &game_timer,
		const DifficultySystem &difficulty_system,
		const EnemeyPrototypeMap &prototypes
	);

	void update(World &world) override;

private:
	v2 get_spawn_offset();

	Rnd rnd;

	f32 spawn_radius;
	f32 respawn_radius_sq;

	const GameTimer &game_timer;
	const DifficultySystem &difficulty_system;

	const EnemeyPrototypeMap &prototypes;
};
