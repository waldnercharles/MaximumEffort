#pragma once
#include "game_state.h"

struct GameStateLoadingLevel final : public GameState
{
	void enter(Game &game) override;
	GameState *update(Game &game) override;
	void exit(Game &game) override;

	void draw_world(Game &game) override;
	void draw_ui(Game &game) override;
};