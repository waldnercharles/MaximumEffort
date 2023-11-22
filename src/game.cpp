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

struct Vertex
{
	float x, y;
	float u, v;
};

static void s_quad(float x, float y, float sx, float sy, Vertex quad[6])
{
	// clang-format off
	quad[0].x = -0.5f; quad[0].y =  0.5f; quad[0].u = 0; quad[0].v = 0;
	quad[1].x =  0.5f; quad[1].y = -0.5f; quad[1].u = 1; quad[1].v = 1;
	quad[2].x =  0.5f; quad[2].y =  0.5f; quad[2].u = 1; quad[2].v = 0;

	quad[3].x = -0.5f; quad[3].y =  0.5f; quad[3].u = 0; quad[3].v = 0;
	quad[4].x = -0.5f; quad[4].y = -0.5f; quad[4].u = 0; quad[4].v = 1;
	quad[5].x =  0.5f; quad[5].y = -0.5f; quad[5].u = 1; quad[5].v = 1;

	for (int i = 0; i < 6; ++i)
	{
		quad[i].x = quad[i].x * sx + x;
		quad[i].y = quad[i].y * sy + y;
	}
	// clang-format on
}

Game::Game()
	: world(),
	  enemy_aabb_grid(720, 720, 16),
	  damage_numbers(std::make_shared<DamageNumbers>(world, event_bus)),
	  damage_system(std::make_shared<DamageSystem>(world, event_bus)),
	  lifetime_system(std::make_shared<LifetimeSystem>()),
	  weapon_system(std::make_shared<WeaponSystem>(enemy_aabb_grid)),
	  spawner_system(std::make_shared<SpawnerSystem>(
		  cf_max(640, 480) * 0.66f,
		  cf_max(640, 480)
	  )),
	  input_system(std::make_shared<InputSystem>()),
	  movement_behavor_system(std::make_shared<MovementBehaviorSystem>()),
	  movement_system(std::make_shared<MovementSystem>()),
	  physics_system(std::make_shared<PhysicsSystem>(enemy_aabb_grid)),
	  projectile_system(
		  std::make_shared<ProjectileSystem>(event_bus, enemy_aabb_grid)
	  ),
	  player_animation_system(std::make_shared<PlayerAnimationSystem>()),
	  camera_system(std::make_shared<CameraSystem>(640, 480)),
	  render_system(std::make_shared<RenderSystem>())
{
	register_scene_node_callbacks(world);

	CF_TextureParams texture_params = cf_texture_defaults();
	texture_params.render_target = true;
	texture_params.width = 640;
	texture_params.height = 480;
	texture_params.wrap_u = CF_WRAP_MODE_CLAMP_TO_EDGE;
	texture_params.wrap_v = CF_WRAP_MODE_CLAMP_TO_EDGE;
	texture_params.filter = CF_FILTER_NEAREST;

	CF_Texture texture = cf_make_texture(texture_params);

	CF_Mesh m = cf_make_mesh(CF_USAGE_TYPE_IMMUTABLE, sizeof(Vertex) * 6, 0, 0);
	CF_VertexAttribute attrs[2] = {};
	attrs[0].name = "in_pos";
	attrs[0].format = CF_VERTEX_FORMAT_FLOAT2;
	attrs[0].offset = CF_OFFSET_OF(Vertex, x);
	attrs[1].name = "in_uv";
	attrs[1].format = CF_VERTEX_FORMAT_FLOAT2;
	attrs[1].offset = CF_OFFSET_OF(Vertex, u);
	cf_mesh_set_attributes(m, attrs, CF_ARRAY_SIZE(attrs), sizeof(Vertex), 0);
	Vertex quad[6];
	s_quad(0, 0, 2, 2, quad);
	cf_mesh_update_vertex_data(m, quad, 6);

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

	damage_numbers->update();
}

void Game::draw()
{

	camera_system->update(world);
	render_system->update(world);

	// TODO: Rendering should maybe be done per-state?
	damage_numbers->draw();

	if (ImGui::Button("Pause"))
	{
		paused = !paused;
	}
}
