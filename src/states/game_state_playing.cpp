#include "states/game_state_playing.h"
#include "cmp/frame_destroy_component.h"
#include "game.h"

void GameStatePlaying::enter(Game &game)
{
	game.difficulty_system.difficulty.health_time_modifier = 4.f;
	game.difficulty_system.difficulty.speed_time_modifier = 1.f;
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
