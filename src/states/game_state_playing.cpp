#include "states/game_state_playing.h"
#include "game.h"

void GameStatePlaying::enter(Game &)
{
}

GameState *GameStatePlaying::update(Game &game)
{
	game.lifetime_system->update(game.world);
	game.weapon_system->update(game.world);
	game.spawner_system->update(game.world);
	game.input_system->update(game.world);
	game.movement_behavor_system->update(game.world);
	game.movement_system->update(game.world);
	game.physics_system->update(game.world);
	game.player_animation_system->update(game.world);
	return this;
}

void GameStatePlaying::exit(Game &)
{
}
