#include "leveling_overlay.h"

bool LevelingOverlay::update()
{
	bool res = false;

	ui.begin();
	int next_id = 1;
	for (auto &p : passives)
	{
		(void)p;
		if (ui.button(next_id++, {}, "Hello PooPooPeePee", 5.f, {1, 1, 1, 1}))
		{
			res |= true;
		}
	}

	ui.end();

	return res;
}

void LevelingOverlay::draw()
{
	ui.draw();

	//	cf_draw_push_color(cf_color_white());
	//	for (auto &p : passives)
	//	{
	//		(void)p;
	//		cf_draw_box2({0, 0}, {100, 0}, {100, 100}, {0, 100}, 1.f, 1.f);
	//	}
	//	cf_draw_pop_color();
}

void LevelingOverlay::set_passives(Array<Passive> &p)
{
	passives.clear();
	passives.steal_from(p);
}
