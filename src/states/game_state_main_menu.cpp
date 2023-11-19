#include "states/game_state_main_menu.h"
#include "assets.h"
#include "factories.h"
#include "game.h"
#include "prefabs/player_prefab.h"

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
	mount_assets_folder();

	tiled_map = game.world.create();
	game.world.emplace<TiledMap>(tiled_map, load_tiled_map("map.json"));

	player = prefabs::Player::create(game.world);

	make_enemy_spawner(game.world, player, 0.001f, ENEMY_EYEBALL);
	make_weapon_boomerang(game.world, player);
}
