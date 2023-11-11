#include <cute.h>

#include "assets.h"
#include "game.h"
#include "imgui.h"
#include "log.h"

#ifdef _WIN32
// Use dedicated GPU unless there is an application-specific override
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

using namespace Cute;

void update(void *udata)
{
	game.update(CF_DELTA_TIME_FIXED);
}

int main(int argc, char *argv[])
{
	clear_color(0.5, 0.5, 0.5, 1);

	int options = APP_OPTIONS_WINDOW_POS_CENTERED | APP_OPTIONS_D3D11_CONTEXT;
	auto result = make_app("Maximum Effort", 0, 0, 1280, 720, options);
	if (is_error(result))
	{
		log_fatal(result.details);
		return -1;
	}

	make_game();

	app_init_imgui();

	set_fixed_timestep(60);

	cf_set_update_udata(&game);

	const f32 alpha = 0.75f;
	f32 fps = 60;

	while (app_is_running())
	{
		app_update(update);

		fps = fps * alpha + (1.0 - alpha) * (1.0 / DELTA_TIME);
		ImGui::Text("%f", fps);

		game.draw();
		app_draw_onto_screen();
	}

	destroy_app();

	return 0;
}