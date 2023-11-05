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

		player.update(dt);

		for (auto &spawner : spawners)
		{
			spawner.update(dt);
		}

		for (auto &weapon : weapons)
		{
			weapon.update(dt);
		}

		for (auto &projectile : projectiles)
		{
			projectile.update(dt);
		}

		for (auto &enemy : enemies)
		{
			enemy.update(dt);
		}

		// Remove dead entities
		for (int i = projectiles.count() - 1; i >= 0; i--)
		{
			if (!projectiles[i].alive)
			{
				projectiles.unordered_remove(i);
			}
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

	for (auto &projectile : projectiles)
	{
		projectile.draw();
	}

	player.draw();


	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}

void make_game()
{
	game.weapons.clear();
	game.projectiles.clear();
	game.enemies.clear();
	game.spawners.clear();
	game = {};

	game.rnd = rnd_seed((u64)time(nullptr));
	game.camera_size = V2(320, 180);

	game.map = load_tiled_map("map.json");

	game.player = {};
	game.player.sprite = make_sprite("character.ase");

	Projectile boomerang = {};
	boomerang.speed = V2(80, 80);
	boomerang.lifetime = 1;
	boomerang.sprite = make_sprite("boomerang.ase");

	Weapon weapon = {};
	weapon.projectile_template = boomerang;
	weapon.rate = 0.5f;

	game.weapons.add(weapon);

	EnemySpawner enemy_spawner = {0.0f, 0.2f, ENEMY_TYPE_EYEBALL};
	game.spawners.add(enemy_spawner);

	//	game.paused = true;
}
