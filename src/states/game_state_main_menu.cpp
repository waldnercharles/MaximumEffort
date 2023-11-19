#include "states/game_state_main_menu.h"
#include "assets.h"
#include "enemy_type.h"
#include "game.h"
#include "prefabs/enemy_spawner_prefab.h"
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
	mount_assets_folder();

	tiled_map = game.world.create();
	game.world.emplace<TiledMap>(tiled_map, load_tiled_map("map.json"));

	player = prefabs::Player::create(game.world);
	eyeball_spawner = prefabs::EnemySpawner::create(
		game.world,
		player,
		0.1f,
		ENEMY_TYPE_EYEBALL
	);

	weapon_boomerang = prefabs::WeaponBoomerang::create(game.world, player);
}
