#pragma once

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

private:
	char text[5];

	float initial_time;
	float elapsed;
	float time_remaining;
};
