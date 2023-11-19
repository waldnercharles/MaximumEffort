#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "states/game_state.h"
#include "states/game_state_main_menu.h"
#include "states/game_state_playing.h"
#include "sys/camera_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/player_animation_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include "tiled_map.h"

struct Game
{
	Game();
	void update();
	void draw();

	friend struct GameStateMainMenu;
	friend struct GameStatePlaying;
	friend struct GameState;

	struct
	{
		GameStateMainMenu main_menu;
		GameStatePlaying playing;
		GameState *current;
	} states;

	bool paused = false;
	bool exit = false;

	World world;

	AabbGrid<Entity> enemy_aabb_grid;

private:
	std::shared_ptr<LifetimeSystem> lifetime_system;
	std::shared_ptr<WeaponSystem> weapon_system;
	std::shared_ptr<SpawnerSystem> spawner_system;
	std::shared_ptr<InputSystem> input_system;
	std::shared_ptr<MovementBehaviorSystem> movement_behavor_system;
	std::shared_ptr<MovementSystem> movement_system;
	std::shared_ptr<PhysicsSystem> physics_system;

	std::shared_ptr<PlayerAnimationSystem> player_animation_system;

	std::shared_ptr<CameraSystem> camera_system;
	std::shared_ptr<RenderSystem> render_system;
};
