#pragma once

#include "passive_tree.h"
#include "ui.h"

struct LevelingOverlay
{
	bool update();
	void draw();

	void set_passives(Array<Passive> &);

private:
	UI ui;

	Array<Passive> passives;
};