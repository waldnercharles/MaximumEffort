#pragma once
#include "common.h"
#include "system.h"

struct SpawnerSystem final : public System
{
	SpawnerSystem(f32 spawn_radius, f32 respawn_radius);
	void update(World &world) override;

	f32 spawn_radius;
	f32 respawn_radius;

private:
	v2 get_spawn_pos();

	Rnd rnd;
};
