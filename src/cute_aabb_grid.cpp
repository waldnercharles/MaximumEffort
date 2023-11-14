#include "cute_aabb_grid.h"
#include <cute.h>

using namespace Cute;

struct CF_AabbGridNodeInternal
{
	int id = 0;
	Aabb aabb = {};
	void *udata = nullptr;
};

struct CF_AabbGridInternal
{
	int w = 0;
	int h = 0;

	v2 pos = {};

	int tile_size = 0;

	Array<Array<CF_AabbGridNodeInternal>> map = {};

	int next_id = 0;
};


CF_AabbGrid cf_make_aabb_grid(CF_V2 pos, int width, int height, int tile_size)
{
	CF_AabbGridInternal *grid = CF_NEW(CF_AabbGridInternal);

	grid->pos = pos;
	grid->w = width;
	grid->h = height;
	grid->tile_size = tile_size;
	grid->next_id = 0;

	int max_tiles_x = width / tile_size;
	int max_tiles_y = height / tile_size;

	grid->map.ensure_count(max_tiles_x * max_tiles_y);

	CF_AabbGrid result;
	result.id = (uint64_t)grid;
	return result;
}

void cf_destroy_aabb_grid(CF_AabbGrid grid_handle)
{
	CF_AabbGridInternal *grid = (CF_AabbGridInternal *)grid_handle.id;
	grid->~CF_AabbGridInternal();
	cf_free(grid);
}

void cf_aabb_grid_clear(CF_AabbGrid grid_handle)
{
	CF_AabbGridInternal *grid = (CF_AabbGridInternal *)grid_handle.id;
	for (int i = 0; i < grid->map.count(); i++)
	{
		grid->map[i].clear();
	}

	grid->next_id = 0;
}

void cf_aabb_grid_set_pos(CF_AabbGrid grid_handle, CF_V2 pos)
{
	CF_AabbGridInternal *grid = (CF_AabbGridInternal *)grid_handle.id;
	grid->pos = pos;
}

void cf_aabb_grid_insert(CF_AabbGrid grid_handle, CF_Aabb aabb, void *udata)
{
	CF_AabbGridInternal *grid = (CF_AabbGridInternal *)grid_handle.id;
	v2 half_extents = V2((float)grid->w, (float)grid->h) * 0.5f;

	int max_tiles_x = grid->w / grid->tile_size;
	int max_tiles_y = grid->h / grid->tile_size;

	Aabb relative_aabb = aabb;
	relative_aabb.min += half_extents - grid->pos;
	relative_aabb.max += half_extents - grid->pos;

	int min_x = (int)relative_aabb.min.x / grid->tile_size;
	int min_y = (int)relative_aabb.min.y / grid->tile_size;

	int max_x = (int)relative_aabb.max.x / grid->tile_size;
	int max_y = (int)relative_aabb.max.y / grid->tile_size;

	if (min_x < 0 || min_y < 0 || max_x >= max_tiles_x || max_y >= max_tiles_y)
	{
		return;
	}

	int id = ++grid->next_id;

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			int index = y * max_tiles_x + x;
			grid->map[index].add({id, aabb, udata});
		}
	}
}

void cf_aabb_grid_query(
	CF_AabbGrid grid_handle,
	CF_Aabb a_aabb,
	CF_AabbGridQueryFn *fn,
	void *udata
)
{
	static Array<int> visited;
	visited.clear();

	CF_AabbGridInternal *grid = (CF_AabbGridInternal *)grid_handle.id;

	v2 half_extents = V2((float)grid->w, (float)grid->h) * 0.5f;

	int max_tiles_x = grid->w / grid->tile_size;
	int max_tiles_y = grid->h / grid->tile_size;

	a_aabb.min += half_extents - grid->pos;
	a_aabb.max += half_extents - grid->pos;

	int min_x = (int)a_aabb.min.x / grid->tile_size;
	int min_y = (int)a_aabb.min.y / grid->tile_size;

	int max_x = (int)a_aabb.max.x / grid->tile_size;
	int max_y = (int)a_aabb.max.y / grid->tile_size;

	if (min_x < 0 || min_y < 0 || max_x >= max_tiles_x || max_y >= max_tiles_y)
	{
		return;
	}

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			int index = y * max_tiles_x + x;

			for (auto b : grid->map[index])
			{
				bool already_visited = false;
				for (auto v : visited)
				{
					if (v == b.id)
					{
						already_visited = true;
						break;
					}
				}

				if (already_visited)
				{
					continue;
				}

				auto b_aabb = b.aabb;
				b_aabb.min += half_extents - grid->pos;
				b_aabb.max += half_extents - grid->pos;

				if (aabb_to_aabb(a_aabb, b_aabb))
				{
					visited.add(b.id);
					if (!fn({b.id}, b.aabb, b.udata, udata))
					{
						return;
					}
				}
			}
		}
	}
}