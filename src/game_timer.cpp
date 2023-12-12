#include "game_timer.h"
#include <charconv>
#include <cute.h>

GameTimer::GameTimer(float initial_time)
	: initial_time(initial_time),
	  elapsed(0),
	  time_remaining(initial_time)
{
}

void GameTimer::update()
{
	elapsed += CF_DELTA_TIME_FIXED;
	time_remaining -= CF_DELTA_TIME_FIXED;

	if (cf_key_just_pressed(CF_KEY_SPACE))
	{
		elapsed += 30;
		time_remaining -= 30;
	}
}

void GameTimer::draw()
{
	const int app_height = cf_app_get_height();

	int minutes = time_remaining / 60;
	int seconds = fmodf(time_remaining, 60);
	sprintf(text, "%02d:%02d", minutes, seconds);

	cf_draw_push_layer(4096);
	cf_push_font("ProggyClean");
	const float font_size = 24.f;
	cf_push_font_size(font_size);

	float text_width = cf_text_width(text);
	CF_V2 text_pos = cf_v2(-text_width * .5f, app_height * .5f);

	cf_push_font_blur(1);
	cf_draw_push_color({0.f, 0.f, 0.f, 1.f});
	cf_draw_text(text, text_pos, -1);
	cf_pop_font_blur();
	cf_draw_pop_color();

	cf_draw_push_color({1.f, 1.f, 1.f, 1.f});
	cf_draw_text(text, text_pos, -1);
	cf_draw_pop_color();

	cf_pop_font_size();

	cf_pop_font();
	cf_draw_pop_layer();
}
