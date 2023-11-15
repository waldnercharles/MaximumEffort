#include "game.h"
#include "assets.h"
#include "camera.h"
#include "cmp/c_enemy_component.h"
#include "common.h"
#include "factories.h"
#include "imgui.h"
#include "sys/behavior_constant_direction_system.h"
#include "sys/behavior_follow_target_system.h"
#include "sys/debug_draw_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/transform_system.h"
#include "sys/weapon_system.h"
#include <ctime>
#include <flecs.h>

using namespace Cute;

Game game;

ECS_COMPONENT_DECLARE(vec2);

void make_game()
{
	game.rnd = rnd_seed((u64)time(nullptr));
	game.camera_size = V2(320, 180);
	game.spawn_radius = max(game.camera_size.x, game.camera_size.y) * 0.66f;
	game.world_size = V2(game.spawn_radius, game.spawn_radius) * 4.0f;

	game.enemy_grid = {
		(int)ceil(game.world_size.x),
		(int)ceil(game.world_size.y),
		{},
		32,
		{}};

	game.world = NEW(flecs::world);
	//	game.world->import <flecs::monitor>();
	//	game.world->set<flecs::Rest>({});

	add_lifetime_system(game.world);

	add_weapon_system(game.world);
	add_spawner_system(game.world);

	add_input_system(game.world);
	add_movement_system(game.world);
	add_behavior_constant_direction_system(game.world);
	add_behavior_follow_target_system(game.world);

	add_transform_system(game.world);

	//	add_physics_system();

	add_player_animation_system(game.world);

	// TODO: Do this somewhere else
	{
		mount_assets_folder();
		game.map = load_tiled_map("map.json");

		flecs::entity player = make_player(game.world);

		make_enemy_spawner(game.world, player, 0.001f, ENEMY_EYEBALL);
		make_weapon_boomerang(game.world, player);
	}

	game.paused = true;
}

//void destroy_game(Game g)
//{
//	unload_tiled_map(g.map);
//}

void Game::update(float dt)
{
	//	world->remove_all<Region>();
	if (!world->progress(dt))
	{
		// exit?
	}

	physics_system(world);


	//	physics_system(*world);

	//	if (!paused)
	//	{
	//		total_time += dt;
	//		lifetime_system(reg, dt);
	//
	//		// Spawn projectiles and enemies
	//		{
	//			weapon_system(reg, dt);
	//			spawner_system(reg, dt);
	//		}
	//
	//		// Handle input and movement
	//		{
	//			input_system(reg, dt);
	//			movement_behavior_follow_target_system(reg, dt);
	//			movement_behavior_constant_direction_system(reg, dt);
	//
	//			// Physics before movement
	//			physics_system(reg, dt);
	//
	//			movement_system(reg, dt);
	//		}
	//
	//		// Handle animation
	//		{
	//			player_animation_system(reg, dt);
	//		}
	//	}
}


void Game::draw()
{
	update_camera(game.world);
	//	camera_system(reg);

	map.draw();

	debug_draw(game.world);
	render_system(game.world);

	auto count = game.world->filter<C_Enemy>().count();

	ImGui::Text("Enemies: %i", count);

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}
