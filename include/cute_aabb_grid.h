#pragma once

#include <cute.h>

typedef struct CF_AabbGrid
{
	uint64_t id;
} CF_AabbGrid;

typedef struct CF_AabbGridNode
{
	int id;
} CF_AabbGridNode;

typedef bool(CF_AabbGridQueryFn)(
	CF_AabbGridNode node,
	CF_Aabb aabb,
	void *leaf_udata,
	void *fn_udata
);

CF_AabbGrid cf_make_aabb_grid(CF_V2 pos, int width, int height, int grid_size);

void cf_destroy_aabb_grid(CF_AabbGrid grid_handle);

void cf_aabb_grid_clear(CF_AabbGrid grid);

void cf_aabb_grid_set_pos(CF_AabbGrid grid_handle, CF_V2 pos);

void cf_aabb_grid_insert(CF_AabbGrid grid, CF_Aabb aabb, void *udata);

void cf_aabb_grid_query(
	CF_AabbGrid grid,
	CF_Aabb aabb,
	CF_AabbGridQueryFn *fn,
	void *udata
);

namespace Cute
{
	using AabbGrid = CF_AabbGrid;
	using AabbGridNode = CF_AabbGridNode;
	using AabbGridQueryFn = CF_AabbGridQueryFn;
	using Aabb = CF_Aabb;
	using v2 = CF_V2;

	CF_INLINE AabbGrid
	make_aabb_grid(v2 pos, int width, int height, int grid_size)
	{
		return cf_make_aabb_grid(pos, width, height, grid_size);
	}

	CF_INLINE void destroy_aabb_grid(AabbGrid grid)
	{
		cf_destroy_aabb_grid(grid);
	}

	CF_INLINE void aabb_grid_clear(AabbGrid grid)
	{
		cf_aabb_grid_clear(grid);
	}

	CF_INLINE void aabb_grid_set_pos(AabbGrid grid, v2 pos)
	{
		cf_aabb_grid_set_pos(grid, pos);
	}

	CF_INLINE void aabb_grid_insert(AabbGrid grid, Aabb aabb, void *udata)
	{
		cf_aabb_grid_insert(grid, aabb, udata);
	}

	CF_INLINE void
	aabb_grid_query(AabbGrid grid, Aabb aabb, AabbGridQueryFn *fn, void *udata)
	{
		cf_aabb_grid_query(grid, aabb, fn, udata);
	}
}
