#pragma once
#include "common.h"
#include "game_state.h"

struct GameStateMainMenu final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;

private:
	Entity tiled_map = ECS_NULL;

	Entity player = ECS_NULL;
	Entity eyeball_spawner = ECS_NULL;
	Entity weapon_boomerang = ECS_NULL;
};
