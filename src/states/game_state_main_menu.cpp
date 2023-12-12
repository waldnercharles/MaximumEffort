#include "states/game_state_main_menu.h"
#include "game.h"
#include "level_1.h"
#include "prefabs/player_prefab.h"
#include "prefabs/weapon_boomerang_prefab.h"

void GameStateMainMenu::enter(Game &game)
{
	game.world.clear();
}

GameState *GameStateMainMenu::update(Game &game)
{
	return &game.states.playing;
}

void GameStateMainMenu::exit(Game &game)
{
}
