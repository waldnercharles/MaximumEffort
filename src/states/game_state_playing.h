#pragma once
#include "events/subscription.h"
#include "game_state.h"

struct GameStatePlaying final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;
	void draw_world(Game &game) override;
	void draw_ui(Game &game) override;

private:
	enum
	{
		PLAYING,
		LEVELING,
		PAUSED
	} state;

	Array<Subscription> subscriptions;
};
