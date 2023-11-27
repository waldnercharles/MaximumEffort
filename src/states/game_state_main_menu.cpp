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
	tiled_map = game.world.create();
	game.world.emplace<TiledMap>(tiled_map, load_tiled_map("map.json"));

	player = prefabs::Player::create(game.world);
	make_level_1(game.world, player);


	weapon_boomerang = prefabs::WeaponBoomerang::create(game.world, player);
}
