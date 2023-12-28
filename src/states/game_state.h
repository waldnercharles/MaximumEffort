#pragma once

struct Game;

struct GameState
{
	virtual void enter(Game &) = 0;
	virtual GameState *update(Game &) = 0;
	virtual void exit(Game &) = 0;

	virtual void draw_world(Game &) = 0;
	virtual void draw_ui(Game &) = 0;
};
