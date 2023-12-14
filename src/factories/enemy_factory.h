#pragma once
#include "common.h"

struct EnemyFactory
{
	EnemyFactory(World &world, Rnd &rnd);
	EnemyFactory(World &world, Rnd &rnd, const char *dir);

	Entity create(const char *name, v2 pos);

	void load(const char *dir);

private:
	World &world;
	Rnd &rnd;

	const char *dir = nullptr;

	Map<const char *, Func<Entity, v2>> prototype_fn;

	Func<Entity, v2> load_prototype(const char *path);
};
