#include "states/game_state_playing.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/health_component.h"
#include "enemies.h"
#include "game.h"
#include "level_1.h"
#include "log.h"
#include "prefabs/player_prefab.h"
#include "prefabs/weapon_boomerang_prefab.h"

void GameStatePlaying::enter(Game &game)
{
	game.difficulty_system.difficulty.health_time_modifier = 4.f;
	game.difficulty_system.difficulty.speed_time_modifier = 1.f;

	const char *dir = "enemies/prototypes/";
	const char **files = cf_fs_enumerate_directory(dir);
	const char **iter;

	for (iter = files; *iter != NULL; iter++)
	{
		char *path = cf_sappend(cf_sset(NULL, dir), *iter);
		game.enemy_prototypes.insert(
			sintern(cf_path_get_filename_no_ext(*iter)),
			load_enemy_prototype(path)
		);
	}

	cf_fs_free_enumerated_directory(files);

	auto tiled_map = game.world.create();
	game.world.emplace<TiledMap>(tiled_map, load_tiled_map("map.json"));

	auto player = prefabs::Player::create(game.world);
	make_level_1(game);

	prefabs::WeaponBoomerang::create(game.world, player);
}

GameState *GameStatePlaying::update(Game &game)
{
	game.game_timer.update();
	game.lifetime_system.update(game.world);

	game.spawner_system.update(game.world);
	game.difficulty_system.update(game.world);
	game.stats_system.update(game.world);

	game.input_system.update(game.world);
	game.movement_behavior_system.update(game.world);
	game.movement_system.update(game.world);
	game.physics_system.update(game.world);

	game.weapon_system.update(game.world);
	game.projectile_system.update(game.world);
	game.hitbox_immunity_system.update(game.world);

	game.player_animation_system.update(game.world);

	auto dead_entities = game.world.view<FrameDestroyComponent>();
	game.world.destroy(dead_entities.begin(), dead_entities.end());

	return this;
}

void GameStatePlaying::exit(Game &game)
{
}
