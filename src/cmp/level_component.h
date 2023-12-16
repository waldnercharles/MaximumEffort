#pragma once

struct C_Level
{
	int level;
	int xp;
	int xp_level_threshold;

	C_Level() : level(0), xp(0), xp_level_threshold(get_xp_to_next_level())
	{
	}

	inline int get_xp_to_next_level()
	{
		float constant = 2.6f;
		float exponent = 2.02f;

		return floor(pow((level + 1) * constant, exponent));
	}
};
