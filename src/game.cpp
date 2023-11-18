#include "game.h"
#include "assets.h"
#include "cmp/enemy.h"
#include "cmp/transform.h"
#include "common.h"
#include "factories.h"
#include "imgui.h"
#include "sys/camera_system.h"
#include "sys/debug_draw_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_constant_direction_system.h"
#include "sys/movement_behavior_follow_target_system.h"
#include "sys/movement_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include <ctime>

Game game;

void make_game()
{
	register_scene_node_callbacks(game.world);

	game.rnd = cf_rnd_seed((u64)time(nullptr));
	game.camera_size = cf_v2(320, 180);
	game.spawn_radius = cf_max(game.camera_size.x, game.camera_size.y) * 0.66f;

	game.world_size = cf_v2(game.spawn_radius, game.spawn_radius) * 4.0f;

	// TODO: Do this somewhere else
	{
		mount_assets_folder();
		game.map = load_tiled_map("map.json");

		auto player = make_player(game.world);

		make_enemy_spawner(game.world, player, 0.001f, ENEMY_EYEBALL);
		make_weapon_boomerang(game.world, player);
	}

	game.paused = false;
}

void destroy_game(Game g)
{
	unload_tiled_map(g.map);
}

void Game::update(float dt)
{
	if (!paused)
	{
		total_time += dt;
		lifetime_system(world, dt);

		// Spawn projectiles and enemies
		{
			weapon_system(world, dt);
			spawner_system(world, dt);
		}

		// Handle input and movement
		{
			input_system(world, dt);
			movement_behavior_follow_target_system(world, dt);
			movement_behavior_constant_direction_system(world, dt);

			// Physics before movement
			physics_system.update(dt);

			movement_system(world);
		}

		// Handle animation
		{
			player_animation_system(world, dt);
		}
	}
}


void Game::draw()
{
	camera_system(world);

	map.draw();

	debug_draw_system(world);
	render_system(world);

	int count = 0;
	game.world.view<Enemy>().each([&]() {
		count++;
	});

	ImGui::Text("Enemies: %i", count);

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}
