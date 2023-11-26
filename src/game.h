#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "particle_system.h"
#include "rendering/render_target.h"
#include "states/game_state.h"
#include "states/game_state_main_menu.h"
#include "states/game_state_playing.h"
#include "sys/camera_system.h"
#include "sys/damage_system.h"
#include "sys/hit_immunity_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/player_animation_system.h"
#include "sys/projectile_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include "tiled_map.h"

#define INTERNAL_RESOLUTION_X 1024
#define INTERNAL_RESOLUTION_Y 1024

#define CAMERA_RESOLUTION_X 320
#define CAMERA_RESOLUTION_Y 240

struct Game
{
	Game();
	~Game();

	void update();
	void draw();

	void resize();

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
	EventBus event_bus;

	AabbGrid<Entity> enemy_aabb_grid;

	std::shared_ptr<DamageNumbers> damage_numbers;
	std::shared_ptr<DamageSystem> damage_system;

private:
	std::shared_ptr<LifetimeSystem> lifetime_system;
	std::shared_ptr<WeaponSystem> weapon_system;
	std::shared_ptr<SpawnerSystem> spawner_system;
	std::shared_ptr<InputSystem> input_system;
	std::shared_ptr<MovementBehaviorSystem> movement_behavor_system;
	std::shared_ptr<MovementSystem> movement_system;
	std::shared_ptr<PhysicsSystem> physics_system;

	std::shared_ptr<ProjectileSystem> projectile_system;
	std::shared_ptr<HitImmunitySystem> hitbox_immunity_system;

	std::shared_ptr<AnimationSystem> player_animation_system;

	std::shared_ptr<CameraSystem> camera_system;
	std::shared_ptr<RenderSystem> render_system;

	CF_Material blit_material = {};
	CF_Shader blit_shader = {};

	RenderTarget main_render_target = {};
	CF_Mesh main_render_quad = {};
};
