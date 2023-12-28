#include "game_state_loading_level.h"
#include "game.h"
#include "level_1.h"
#include "prefabs/player_prefab.h"
#include "prefabs/weapon_prefab.h"

void GameStateLoadingLevel::enter(Game &game)
{
}

GameState *GameStateLoadingLevel::update(Game &game)
{
	game.difficulty_system.difficulty.health_time_modifier = 4.f;
	game.difficulty_system.difficulty.speed_time_modifier = 1.f;

	auto tiled_map = game.world.create();
	game.world.emplace<TiledMap>(tiled_map, load_tiled_map("map.json"));

	auto player = prefabs::Player::create(game.world);
	make_level_1(game);

	prefabs::Weapon::create(game.world, player);

	return &game.states.playing;
}

void GameStateLoadingLevel::exit(Game &game)
{
}

void GameStateLoadingLevel::draw_world(Game &game)
{
}

void GameStateLoadingLevel::draw_ui(Game &game)
{
}
