#include "states/game_state_playing.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/health_component.h"
#include "events/level_up_event.h"
#include "game.h"

void GameStatePlaying::enter(Game &game)
{
	subscriptions.add(game.event_bus.on<LevelUpEvent>([&](LevelUpEvent e) {
		StatsModifier m = {};
		m.projectile_amount.increase = 1;

		Array<StatsModifier> traits;
		traits.add(m);

		auto &stats = game.world.get<C_Stats>(e.entity);
		stats.add_modifier(traits[0]);
	}));
}

GameState *GameStatePlaying::update(Game &game)
{
	game.game_timer.update();

	game.lifetime_system.update(game.world);
	game.health_system.update(game.world);

	game.spawner_system.update(game.world);
	game.difficulty_system.update(game.world);
	game.health_system.update(game.world);

	game.input_system.update(game.world);

	game.movement_behavior_system.update(game.world);
	game.homing_mover_system.update(game.world);

	game.movement_system.update(game.world);

	game.physics_system.update(game.world);

	game.weapon_system.update(game.world);
	game.projectile_system.update(game.world);
	game.hitbox_immunity_system.update(game.world);

	game.animation_system.update(game.world);

	game.xp_system.update(game.world);
	game.pickup_system.update(game.world);

	auto dead_entities = game.world.view<C_FrameDestroy>();
	if (dead_entities.size() > 0)
	{
		game.world.destroy(dead_entities.begin(), dead_entities.end());
	}

	return this;
}

void GameStatePlaying::exit(Game &game)
{
	for (auto &sub : subscriptions)
	{
		game.event_bus.off(sub);
	}
}
