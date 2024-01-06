#pragma once
#include "common.h"
#include <cute.h>
#include <cute_tiled.h>

struct TiledMap
{
	cute_tiled_map_t *map = nullptr;
	Array<CF_Sprite> atlas;

	v2 pos = {};

	void draw();
};

TiledMap load_tiled_map(const char *path);
void unload_tiled_map(TiledMap tiled_map);