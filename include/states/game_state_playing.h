#pragma once
#include "game_state.h"

struct GameStatePlaying final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;
};
