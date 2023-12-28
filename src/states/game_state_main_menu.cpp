#include "states/game_state_main_menu.h"
#include "game.h"

void GameStateMainMenu::enter(Game &game)
{
	game.world.clear();
}

GameState *GameStateMainMenu::update(Game &game)
{
	return &game.states.loading;
}

void GameStateMainMenu::exit(Game &game)
{
}

void GameStateMainMenu::draw_world(Game &game)
{
}

void GameStateMainMenu::draw_ui(Game &game)
{
}
