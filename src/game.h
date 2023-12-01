#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "damage_numbers.h"
#include "game_timer.h"
#include "rendering/render_target.h"
#include "states/game_state.h"
#include "states/game_state_main_menu.h"
#include "states/game_state_playing.h"
#include "sys/animation_system.h"
#include "sys/camera_system.h"
#include "sys/damage_system.h"
#include "sys/difficulty_system.h"
#include "sys/health_system.h"
#include "sys/hit_immunity_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/projectile_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include "tiled_map.h"

struct Game
{
	const int CAMERA_RESOLUTION_X = 320;
	const int CAMERA_RESOLUTION_Y = 240;

	const float CAMERA_OFFSCREEN_DIST =
		.5f * cf_distance(
				  cf_v2(0, 0),
				  cf_v2(CAMERA_RESOLUTION_X, CAMERA_RESOLUTION_Y)
			  );

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

private:
	GameTimer game_timer;
	DamageNumbers damage_numbers;

	// ECS Systems
	// TODO: Rename to DamageEventHandler?
	DamageSystem damage_system;

	LifetimeSystem lifetime_system;

	DifficultySystem difficulty_system;
	SpawnerSystem spawner_system;
	HealthSystem stats_system;

	InputSystem input_system;
	MovementBehaviorSystem movement_behavior_system;
	MovementSystem movement_system;
	PhysicsSystem physics_system;

	WeaponSystem weapon_system;
	ProjectileSystem projectile_system;
	HitImmunitySystem hitbox_immunity_system;

	AnimationSystem player_animation_system;

	// Rendering
	CameraSystem camera_system;
	RenderSystem render_system;

	CF_Material blit_material = {};
	CF_Shader blit_shader = {};

	RenderTarget main_render_target = {};
	CF_Mesh main_render_quad = {};
};
