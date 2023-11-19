#pragma once
#include "common.h"
#include "game_state.h"

struct GameStateMainMenu final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;

private:
	Entity player = ECS_NULL;
	Entity tiled_map = ECS_NULL;
};
