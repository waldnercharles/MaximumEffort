#pragma once
#include "array_algorithms.h"
#include "common.h"
#include "geometry.h"
#include <glm/glm.hpp>

#include <cute.h>

template <typename T>
struct AabbGridNode
{
	size_t id;
	T value;
};

template <typename T>
struct AabbGrid
{
	AabbGrid(f32 width, f32 height, f32 cell_size);

	void add(Aabb aabb, T value);
	void query(Aabb aabb, Func<bool, T> fn);
	void clear();

	v2 pos;

private:
	const v2 extents;
	const v2 half_extents;

	const f32 cell_size;

	const int max_tiles_x;
	const int max_tiles_y;

	Array<Array<AabbGridNode<T>>> cells;

	size_t next_id;
};

template <typename T>
AabbGrid<T>::AabbGrid(f32 width, f32 height, f32 cell_size)
	: pos(cf_v2(0, 0)),
	  extents(cf_v2(width, height)),
	  half_extents(cf_div_v2_f(extents, 2.f)),
	  cell_size(cell_size),
	  max_tiles_x(width / cell_size),
	  max_tiles_y(height / cell_size),
	  next_id(0)
{
	cells.ensure_count(max_tiles_x * max_tiles_y);
}

template <typename T>
void AabbGrid<T>::add(Aabb aabb, T value)
{

	const v2 min = (aabb.min - pos + half_extents) / cell_size;
	const v2 max = (aabb.max - pos + half_extents) / cell_size;

	if (min.x < 0 || min.y < 0 || max.x >= max_tiles_x || max.y >= max_tiles_y)
	{
		return;
	}

	++next_id;

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			int index = y * max_tiles_x + x;
			cells[index].add({next_id, value});
		}
	}
}

template <typename T>
void AabbGrid<T>::query(Aabb aabb, Func<bool, T> fn)
{

	static Array<size_t> visited;
	visited.clear();

	const v2 min = (aabb.min - pos + half_extents) / cell_size;
	const v2 max = (aabb.max - pos + half_extents) / cell_size;

	if (min.x < 0 || min.y < 0 || max.x >= max_tiles_x || max.y >= max_tiles_y)
	{
		return;
	}

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			int index = y * max_tiles_x + x;
			for (const auto node : cells[index])
			{
				if (arr_contains(visited.begin(), visited.end(), node.id))
				{
					continue;
				}

				visited.add(node.id);
				if (!fn(node.value))
				{
					return;
				}
			}
		}
	}
}

template <typename T>
void AabbGrid<T>::clear()
{
	for (int i = 0; i < cells.count(); i++)
	{
		cells[i].clear();
	}
	next_id = 0;
}
