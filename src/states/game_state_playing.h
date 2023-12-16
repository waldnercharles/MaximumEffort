#pragma once
#include "events/subscription.h"
#include "game_state.h"

struct GameStatePlaying final : public GameState
{
	void enter(Game &) override;
	GameState *update(Game &) override;
	void exit(Game &) override;

private:
	Array<Subscription> subscriptions;

	enum
	{
		PLAYING,
		LEVELING,
	} state;
};
