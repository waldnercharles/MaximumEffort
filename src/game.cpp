#include "game.h"
#include "cmp/transform_component.h"
#include "common.h"

#include "shaders/blit_shader.h"

static void quad_verts(float x, float y, float sx, float sy, Vertex quad[6])
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

static CF_Mesh make_main_render_quad(float x, float y, float sx, float sy)
{
	auto quad = cf_make_mesh(CF_USAGE_TYPE_IMMUTABLE, sizeof(Vertex) * 6, 0, 0);

	CF_VertexAttribute attr[2] = {};
	attr[0].name = "in_pos";
	attr[0].format = CF_VERTEX_FORMAT_FLOAT2;
	attr[0].offset = CF_OFFSET_OF(Vertex, x);
	attr[1].name = "in_uv";
	attr[1].format = CF_VERTEX_FORMAT_FLOAT2;
	attr[1].offset = CF_OFFSET_OF(Vertex, u);
	cf_mesh_set_attributes(quad, attr, CF_ARRAY_SIZE(attr), sizeof(Vertex), 0);

	Vertex verts[6];
	quad_verts(x, y, sx, sy, verts);

	cf_mesh_update_vertex_data(quad, verts, 6);

	return quad;
}

void Game::resize()
{
	int w, h;
	cf_app_get_size(&w, &h);
	w = CF_ALIGN_TRUNCATE(w, 2);
	h = CF_ALIGN_TRUNCATE(h, 2);

	f32 desired_aspect = 4.f / 3.f;
	f32 actual_aspect = (f32)w / (f32)h;
	f32 aspect = (desired_aspect / actual_aspect);

	if (main_render_quad.id)
	{
		cf_destroy_mesh(main_render_quad);
	}
	main_render_quad = make_main_render_quad(0, 0, 2 * aspect, 2);

	cf_app_set_size(w, h);
	cf_app_set_canvas_size(w, h);

	destroy_render_target(main_render_target);
	main_render_target = make_render_target(640, 480);

	cf_material_set_texture_fs(
		blit_material,
		"u_image",
		main_render_target.backbuffer
	);
}

Game::Game()
	: world(),
	  rnd(cf_rnd_seed(time(nullptr))),
	  enemy_factory(world, rnd, "enemies/prototypes/"),
	  enemy_aabb_grid(CAMERA_RESOLUTION_X * 2, CAMERA_RESOLUTION_Y * 2, 16),
	  damage_numbers(world, event_bus),
	  game_timer(1800),
	  damage_system(world, event_bus),
	  lifetime_system(),
	  weapon_system(enemy_aabb_grid, game_timer),
	  difficulty_system({}, game_timer),
	  spawner_system(
		  rnd,
		  game_timer,
		  difficulty_system,
		  enemy_factory,
		  CAMERA_OFFSCREEN_DIST,
		  CAMERA_OFFSCREEN_DIST * 2.f
	  ),
	  stats_system(),
	  input_system(),
	  movement_behavior_system(),
	  movement_system(),
	  physics_system((enemy_aabb_grid)),
	  projectile_system(event_bus, enemy_aabb_grid),
	  hitbox_immunity_system(),
	  player_animation_system(),
	  camera_system(CAMERA_RESOLUTION_X, CAMERA_RESOLUTION_Y),
	  render_system()
{
	register_transform_callbacks(world);

	blit_material = cf_make_material();
	blit_shader = CF_MAKE_SOKOL_SHADER(blit_shader);

	resize();

	states.current = &states.main_menu;
}

Game::~Game()
{
	//	cf_destroy_shader(blit_shader);
	//	cf_destroy_material(blit_material);
	//	destroy_render_target(main_render_target);
	//	cf_destroy_mesh(main_render_quad);
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

	damage_numbers.update();
}

// TODO: This should maybe be per-game-state
void Game::draw()
{
	if (cf_app_was_resized())
	{
		resize();
	}

	// Draw Game
	cf_camera_push();
	{
		camera_system.update(world);
		render_system.update(world);
		damage_numbers.draw();

		cf_render_to(main_render_target.canvas, true);

		// Fetch each frame, as it's invalidated during window-resize
		CF_Canvas app_canvas = cf_app_get_canvas();

		cf_apply_canvas(app_canvas, false);
		{
			// Draw offscreen texture onto the app's canvas on the left.
			cf_apply_mesh(main_render_quad);
			cf_apply_shader(blit_shader, blit_material);
			cf_draw_elements();
		}
	}
	cf_camera_pop();

	// Draw UI
	game_timer.draw();

	cf_app_draw_onto_screen(false);
}