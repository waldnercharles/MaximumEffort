#pragma once
#include <cute_tiled.h>

#include "common.h"
#include "event_handlers/damage_numbers_event_handler.h"
#include "event_handlers/death_event_handler.h"
#include "event_handlers/hit_event_handler.h"
#include "factories/enemy_factory.h"
#include "game_timer.h"
#include "leveling_overlay.h"
#include "passive_tree.h"
#include "rendering/render_target.h"
#include "states/game_state.h"
#include "states/game_state_loading_level.h"
#include "states/game_state_main_menu.h"
#include "states/game_state_playing.h"
#include "sys/animation_system.h"
#include "sys/camera_system.h"
#include "sys/difficulty_system.h"
#include "sys/health_system.h"
#include "sys/hit_immunity_system.h"
#include "sys/homing_mover_system.h"
#include "sys/input_system.h"
#include "sys/lifetime_system.h"
#include "sys/movement_behavior_system.h"
#include "sys/movement_system.h"
#include "sys/physics_system.h"
#include "sys/pickup_system.h"
#include "sys/projectile_system.h"
#include "sys/render_system.h"
#include "sys/spawner_system.h"
#include "sys/weapon_system.h"
#include "sys/xp_system.h"
#include "tiled/tiled_map.h"
#include "ui.h"

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
	friend struct GameStateLoadingLevel;
	friend struct GameStatePlaying;
	friend struct GameState;

	struct
	{
		GameStateMainMenu main_menu;
		GameStateLoadingLevel loading;
		GameStatePlaying playing;
		GameState *current;
	} states;

	bool paused = false;
	bool exit = false;

	World world;
	EventBus event_bus;
	Rnd rnd;

private:
	EnemyFactory enemy_factory;

	GameTimer game_timer;

	AabbGrid<Entity> enemy_aabb_grid;

	UI ui;

	// Event Handlers
	HitEventHandler hit_event_handler;
	DeathEventHandler death_event_handler;
	DamageNumbersEventHandler damage_numbers_event_handler;

	// ECS Systems
	LifetimeSystem lifetime_system;

	DifficultySystem difficulty_system;
	SpawnerSystem spawner_system;
	HealthSystem health_system;

	InputSystem input_system;

	MovementBehaviorSystem movement_behavior_system;
	MovementSystem movement_system;
	HomingMoverSystem homing_mover_system;

	PhysicsSystem physics_system;

	WeaponSystem weapon_system;
	ProjectileSystem projectile_system;
	HitImmunitySystem hitbox_immunity_system;

	AnimationSystem animation_system;

	XpSystem xp_system;
	PickupSystem pickup_system;

	LevelingOverlay leveling_overlay;

	// Rendering
	CameraSystem camera_system;
	RenderSystem render_system;

	CF_Material blit_material = {};
	CF_Shader blit_shader = {};

	RenderTarget main_render_target = {};
	CF_Mesh main_render_quad = {};

	RenderTarget ui_render_target = {};
	CF_Mesh ui_render_quad = {};
};
