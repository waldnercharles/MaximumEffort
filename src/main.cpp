#include <cute.h>

#include "assets.h"
#include "cmp/transform_component.h"
#include "game.h"
#include "imgui.h"
#include "log.h"

#include "http/http_server.h"

#ifdef _WIN32
// Use dedicated GPU unless there is an application-specific override
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif


void update(void *udata)
{
	Game *game = (Game *)udata;
	game->update();
}

int main(int, char *[])
{
	cf_clear_color(0.5, 0.5, 0.5, 1);

	int options = CF_APP_OPTIONS_WINDOW_POS_CENTERED |
				  CF_APP_OPTIONS_D3D11_CONTEXT | CF_APP_OPTIONS_RESIZABLE;

	auto result = cf_make_app("Maximum Effort", 0, 0, 1280, 720, options, NULL);

	if (cf_is_error(result))
	{
		log_fatal(result.details);
		return -1;
	}

	cf_app_set_vsync(true);

	mount_assets_dir();

	// Server should be started after assets have been mounted
	start_http_server_thread();

	cf_make_font("ProggyClean.ttf", "ProggyClean");
	cf_app_init_imgui(false);

	Game game;

	cf_set_fixed_timestep(60);
	cf_set_update_udata(&game);

	const f32 alpha = 0.95f;
	f32 fps = 60;

	while (cf_app_is_running() && !game.exit)
	{
		cf_app_update(update);

		fps = fps * alpha + (1.f - alpha) * (1.f / DELTA_TIME);
		ImGui::Text("%f", fps);

		game.draw();
	}

	cf_destroy_app();

	return 0;
}