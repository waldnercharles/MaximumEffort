#include "states/game_state_playing.h"
#include "cmp/frame_destroy_component.h"
#include "cmp/health_component.h"
#include "events/level_up_event.h"
#include "game.h"
#include "log.h"

Array<int> get_random_indices(Rnd &rnd, int min, int max, int n)
{
	Array<int> res;

	n = std::max(0, std::min(max - min, n));

	for (int i = 0; i < n && i < max - min; i++)
	{
		int r;
		do
		{
			r = cf_rnd_next_range_int(&rnd, min, max - 1);

		} while (arr_contains(res.begin(), res.end(), r));

		res.add(r);
	}

	return res;
}

void GameStatePlaying::enter(Game &game)
{
	state = PLAYING;

	subscriptions.add(game.event_bus.on<LevelUpEvent>([&](LevelUpEvent e) {
		state = LEVELING;

		auto &passive_tree = game.world.get<C_PassiveTree>(e.entity);
		auto &available_passives = passive_tree.get_available_passives();
		if (available_passives.count() == 0)
		{
			log_trace("Level up! {}! No more passives to unlock", e.level);
			return;
		}

		auto indices =
			get_random_indices(game.rnd, 0, available_passives.count(), 3);

		Array<Passive> passives;

		for (auto i : indices)
		{
			passives.add(available_passives[i]);
		}

		game.leveling_overlay.set_passives(passives);

		//		passive_tree.unlock_passive(rnd_passive);
		//
		//		auto &stats = game.world.get<C_Stats>(e.entity);
		//		stats.add_modifier(rnd_passive.stats_modifier);
		//
		//		log_info("Unlocked passive:");
		//		for (const char *desc : rnd_passive.description)
		//		{
		//			log_info(desc);
		//		}
	}));
}

GameState *GameStatePlaying::update(Game &game)
{
	switch (state)
	{
		case PAUSED:
		{
			break;
		}
		case LEVELING:
		{
			if (game.leveling_overlay.update())
			{
				state = PLAYING;
			}
			break;
		}
		case PLAYING:
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
			break;
		}
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

void GameStatePlaying::draw_world(Game &game)
{
	game.camera_system.update(game.world);
	game.render_system.update(game.world);

	game.damage_numbers_event_handler.draw();
}

void GameStatePlaying::draw_ui(Game &game)
{
	game.game_timer.draw();

	if (state == LEVELING)
	{
		game.leveling_overlay.draw();
	}
}
