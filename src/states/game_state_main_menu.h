#pragma once
#include "common.h"
#include "game_state.h"

struct GameStateMainMenu final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;

	void draw_world(Game &game) override;
	void draw_ui(Game &game) override;
};
