#include "states/game_state_playing.h"
#include "cmp/frame_destroy_component.h"
#include "game.h"

void GameStatePlaying::enter(Game &game)
{
}

GameState *GameStatePlaying::update(Game &game)
{
	game.game_timer.update();
	game.hitbox_immunity_system.update(game.world);
	game.lifetime_system.update(game.world);
	game.weapon_system.update(game.world);
	game.spawner_system.update(game.world);
	game.input_system.update(game.world);
	game.movement_behavior_system.update(game.world);
	game.movement_system.update(game.world);
	game.physics_system.update(game.world);
	game.projectile_system.update(game.world);
	game.player_animation_system.update(game.world);

	auto dead_entities = game.world.view<FrameDestroyComponent>();
	game.world.destroy(dead_entities.begin(), dead_entities.end());

	return this;
}

void GameStatePlaying::exit(Game &game)
{
}
