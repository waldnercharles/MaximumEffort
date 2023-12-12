#pragma once
#include <cute.h>

struct GameTimer
{
	GameTimer(float initial_time);

	void update();
	void draw();

	inline float get_initial_time() const
	{
		return initial_time;
	}

	inline float get_elapsed() const
	{
		return elapsed;
	}

	inline float get_time_remaining() const
	{
		return time_remaining;
	}

	inline bool on_interval(float interval) const
	{
		int prev = (int)((elapsed - CF_DELTA_TIME_FIXED) / interval);
		int next = (int)(elapsed / interval);
		return prev < next;
	}

private:
	char text[5];

	float initial_time;
	float elapsed;
	float time_remaining;
};
