#include "game.h"
#include "cmp/scene_node_component.h"
#include "factories.h"
#include "imgui.h"
#include "sys/camera_system.h"
#include "sys/enemy_target_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include <ctime>

using namespace Cute;

Game game = {};

void Game::update(float dt)
{
	if (!paused)
	{
		total_time += dt;
		lifetime_system(reg, dt);

		weapon_system(reg, dt);

		spawner_system(reg, dt);
		enemy_target_system(reg, dt);

		input_system(reg, dt);
		movement_system(reg, dt);

		player_animation_system(reg, dt);

		physics_system(reg, dt);
	}
}


void Game::draw()
{
	camera_system(reg);

	map.draw();

	render_system(reg);

	auto targeting_circle = make_circle(V2(0, 0), 64.f);

	draw_push_color({0, 0, 1, 0.5});
	draw_circle(targeting_circle);
	draw_pop_layer();

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}

void make_game()
{
	if (game.enemy_aabb_tree.id)
	{
		destroy_aabb_tree(game.enemy_aabb_tree);
	}

	register_scene_node_callbacks(game.reg);

	game.rnd = rnd_seed((u64)time(nullptr));
	game.camera_size = V2(320, 180);

	game.map = load_tiled_map("map.json");

	auto player = make_player(game.reg);

	make_enemy_spawner(game.reg, player, 0.2f, ENEMY_EYEBALL);
	make_weapon_boomerang(game.reg, player);

	game.enemy_aabb_tree = make_aabb_tree();
}
