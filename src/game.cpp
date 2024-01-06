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

static CF_Mesh make_render_quad(float x, float y, float sx, float sy)
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

	// Main Render Target
	{
		f32 desired_aspect =
			(f32)CAMERA_RESOLUTION_X / (f32)CAMERA_RESOLUTION_Y;
		f32 actual_aspect = (f32)w / (f32)h;
		f32 aspect = (desired_aspect / actual_aspect);

		if (main_render_quad.id)
		{
			cf_destroy_mesh(main_render_quad);
		}
		main_render_quad = make_render_quad(0, 0, 2 * aspect, 2);

		destroy_render_target(main_render_target);

		int multiplier =
			cf_max(1, cf_max(h / CAMERA_RESOLUTION_Y, w / CAMERA_RESOLUTION_X));

		main_render_target = make_render_target(
			CAMERA_RESOLUTION_X * multiplier,
			CAMERA_RESOLUTION_Y * multiplier
		);
	}

	// UI Render Target
	{
		if (ui_render_quad.id)
		{
			cf_destroy_mesh(ui_render_quad);
		}
		ui_render_quad = make_render_quad(0, 0, 2, 2);

		destroy_render_target(ui_render_target);
		ui_render_target = make_render_target(w, h, CF_FILTER_LINEAR);
	}

	cf_app_set_size(w, h);
	cf_app_set_canvas_size(w, h);
}

Game::Game()
	: world(),
	  rnd(cf_rnd_seed(time(nullptr))),
	  event_bus(),
	  enemy_factory(world, rnd, "enemies/prototypes/"),

	  game_timer(*this, 1800),

	  enemy_aabb_grid(CAMERA_RESOLUTION_X * 2, CAMERA_RESOLUTION_Y * 2, 16),

	  hit_event_handler(world, event_bus),
	  death_event_handler(world, event_bus),
	  damage_numbers_event_handler(world, event_bus),

	  lifetime_system(),
	  weapon_system(enemy_aabb_grid, game_timer),
	  difficulty_system(LevelDifficulty(), game_timer),
	  spawner_system(
		  rnd,
		  game_timer,
		  difficulty_system,
		  enemy_factory,
		  CAMERA_OFFSCREEN_DIST,
		  CAMERA_OFFSCREEN_DIST * 2.f
	  ),
	  health_system(event_bus),
	  input_system(),

	  movement_behavior_system(),
	  homing_mover_system(),
	  movement_system(),

	  physics_system((enemy_aabb_grid)),
	  projectile_system(event_bus, enemy_aabb_grid),
	  hitbox_immunity_system(),
	  animation_system(),
	  xp_system(world, event_bus),
	  pickup_system(event_bus),
	  leveling_overlay(),

	  camera_system(CAMERA_RESOLUTION_X, CAMERA_RESOLUTION_Y),
	  render_system()
{
	register_transform_callbacks(world);

	auto state = cf_render_state_defaults();

	state.blend.enabled = true;
	state.blend.rgb_src_blend_factor = CF_BLENDFACTOR_SRC_ALPHA;
	state.blend.rgb_dst_blend_factor = CF_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	state.blend.rgb_op = CF_BLEND_OP_ADD;
	state.blend.alpha_src_blend_factor = CF_BLENDFACTOR_SRC_ALPHA;
	state.blend.alpha_dst_blend_factor = CF_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	state.blend.alpha_op = CF_BLEND_OP_ADD;

	blit_material = cf_make_material();
	blit_shader = CF_MAKE_SOKOL_SHADER(blit_shader);

	cf_material_set_render_state(blit_material, state);

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

	damage_numbers_event_handler.update();
}

// TODO: This should maybe be per-game-state
void Game::draw()
{
	if (cf_app_was_resized())
	{
		resize();
	}

	// Fetch canvas each frame, because it's invalidated during window-resize
	CF_Canvas app_canvas = cf_app_get_canvas();

	// Draw Game
	cf_clear_color(0.5, 0.5, 0.5, 1);
	cf_camera_push();
	{
		// Draw to the main render target
		{
			states.current->draw_world(*this);
			cf_render_to(main_render_target.canvas, true);
		}

		// Copy to the app canvas
		cf_apply_canvas(app_canvas, false);
		{
			cf_material_set_texture_fs(
				blit_material,
				"u_image",
				main_render_target.backbuffer
			);

			// Draw offscreen texture onto the app's canvas
			cf_apply_mesh(main_render_quad);
			cf_apply_shader(blit_shader, blit_material);
			cf_draw_elements();
		}
	}
	cf_camera_pop();

	// Draw UI
	cf_clear_color(0, 0, 0, 0);
	cf_camera_push();
	{
		// Draw to the ui render target
		{
			states.current->draw_ui(*this);
			cf_render_to(ui_render_target.canvas, true);
		}

		// Draw to the app canvas
		cf_apply_canvas(app_canvas, false);
		{
			cf_material_set_texture_fs(
				blit_material,
				"u_image",
				ui_render_target.backbuffer
			);

			// Draw offscreen texture onto the app's canvas
			cf_apply_mesh(ui_render_quad);
			cf_apply_shader(blit_shader, blit_material);
			cf_draw_elements();
		}
	}
	cf_camera_pop();

	cf_app_draw_onto_screen(false);
}