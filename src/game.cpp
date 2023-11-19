#include "game.h"
#include "assets.h"
#include "cmp/enemy_component.h"
#include "cmp/transform_component.h"
#include "common.h"
#include "imgui.h"
#include "sys/camera_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_system.h"
#include "sys/movement_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"

Game::Game()
	: world(),
	  enemy_aabb_grid(480, 480, 16),
	  lifetime_system(std::make_shared<LifetimeSystem>()),
	  weapon_system(std::make_shared<WeaponSystem>(enemy_aabb_grid)),
	  spawner_system(std::make_shared<SpawnerSystem>(
		  cf_max(320, 180) * 0.66f,
		  cf_max(320, 180)
	  )),
	  input_system(std::make_shared<InputSystem>()),
	  movement_behavor_system(std::make_shared<MovementBehaviorSystem>()),
	  movement_system(std::make_shared<MovementSystem>()),
	  physics_system(std::make_shared<PhysicsSystem>(enemy_aabb_grid)),
	  player_animation_system(std::make_shared<PlayerAnimationSystem>()),
	  camera_system(std::make_shared<CameraSystem>(320, 180)),
	  render_system(std::make_shared<RenderSystem>())
{
	register_scene_node_callbacks(world);

	states.current = &states.main_menu;
}

void Game::update()
{
	GameState *new_state = states.current->update(*this);
	if (states.current != new_state)
	{
		states.current->exit(*this);
		new_state->enter(*this);
		states.current = new_state;
	}
}

void Game::draw()
{
	camera_system->update(world);
	render_system->update(world);

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}
