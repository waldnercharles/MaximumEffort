#include "game.h"
#include "imgui.h"
#include <ctime>

using namespace Cute;

Game game = {};

void Game::update(float dt)
{
	if (!paused)
	{
		total_time += dt;
		spawn_timer += dt;

		player.update(dt);

		for (auto &spawner : spawners)
		{
			spawner.update(dt);
		}

		for (auto &enemy : enemies)
		{
			enemy.update(dt);
		}
	}
}

void Game::draw()
{
	camera_dimensions(game.camera_size.x, game.camera_size.y);
	auto dim = cf_camera_peek_dimensions();
	ImGui::Text("%f %f", dim.x, dim.y);
	camera_look_at(player.pos);

	map.draw();

	for (auto &enemy : enemies)
	{
		enemy.draw();
	}

	player.draw();


	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}

void make_game()
{
	game.rnd = rnd_seed((u64)time(nullptr));
	game.camera_size = V2(320, 180);

	game.player = {};
	game.player.sprite = make_sprite("character.ase");
	game.map = load_tiled_map("map.json");

	EnemySpawner enemy_spawner = {0.0f, 0.2f, ENEMY_TYPE_EYEBALL};

	game.spawners.add(enemy_spawner);
}
