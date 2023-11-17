#include "game.h"
#include "assets.h"
#include "cmp/c_enemy_component.h"
#include "cmp/c_transform.h"
#include "common.h"
#include "factories.h"
#include "imgui.h"
#include "singleton.h"
#include "sys/camera_system.h"
#include "sys/debug_draw_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_constant_direction_system.h"
#include "sys/movement_behavior_follow_target_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include <ctime>

using namespace Cute;

Game game;

Game make_game()
{
	Game g;

	register_scene_node_callbacks(g.reg);

	g.rnd = rnd_seed((u64)time(nullptr));
	g.camera_size = V2(320, 180);
	g.spawn_radius = max(g.camera_size.x, g.camera_size.y) * 0.66f;
	g.world_size = V2(g.spawn_radius, g.spawn_radius) * 4.0f;
	g.enemy_grid = make_aabb_grid(
		V2(0, 0),
		ceil(g.world_size.x),
		ceil(g.world_size.y),
		32
	);

	// TODO: Do this somewhere else
	{
		mount_assets_folder();
		g.map = load_tiled_map("map.json");

		auto player = make_player(g.reg);

		make_enemy_spawner(g.reg, player, 0.001f, ENEMY_EYEBALL);
		make_weapon_boomerang(g.reg, player);
	}

	g.paused = false;

	return g;
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
		lifetime_system(reg, dt);

		// Spawn projectiles and enemies
		{
			weapon_system(reg, dt);
			spawner_system(reg, dt);
		}

		// Handle input and movement
		{
			input_system(reg, dt);
			movement_behavior_follow_target_system(reg, dt);
			movement_behavior_constant_direction_system(reg, dt);

			// Physics before movement
			physics_system.update(dt);

			movement_system(reg, dt);
		}

		// Handle animation
		{
			player_animation_system(reg, dt);
		}
	}
}


void Game::draw()
{
	camera_system(reg);

	map.draw();

	debug_draw_system(reg);
	render_system(reg);

	int count = 0;
	game.reg.view<C_Enemy>().each([&]() {
		count++;
	});

	ImGui::Text("Enemies: %i", count);

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}
