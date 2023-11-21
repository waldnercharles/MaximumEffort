#pragma once
#include "event.h"
#include "game_state.h"
#include "sys/damage_system.h"

struct GameStatePlaying final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;
};
