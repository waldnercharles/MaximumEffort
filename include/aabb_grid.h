#pragma once
#include "array_algorithms.h"
#include "common.h"
#include <cute.h>

struct AabbCell
{
	int x, y;
};

template <typename T>
struct AabbGridNode
{
	int id;
	T udata;
};

template <typename T>
struct AabbGrid
{
	AabbGrid();
	AabbGrid(int cell_size);

	AabbGrid(const AabbGrid &) = delete;
	AabbGrid &operator=(const AabbGrid &) = delete;

	void clear();
	void insert(Cute::Aabb aabb, T udata);

	void query(Cute::Aabb aabb, Func<bool, T> fn);

private:
	int cell_size;
	Cute::Map<AabbCell, Cute::Array<AabbGridNode<T>>> cells;

	int next_id;

	Cute::Array<int> query_cache;
};

template <typename T>
AabbGrid<T>::AabbGrid() : AabbGrid(16)
{
}

template <typename T>
AabbGrid<T>::AabbGrid(int cell_size) : cell_size(16),
									   cells(),
									   next_id(0)
{
}

template <typename T>
void AabbGrid<T>::clear()
{
	for (int i = 0; i < cells.count(); i++)
	{
		cells.vals()[i].clear();
	}

	next_id = 0;
}

template <typename T>
void AabbGrid<T>::insert(Cute::Aabb aabb, T udata)
{
	++next_id;

	using namespace Cute;
	auto min = (v2)(aabb.min / cell_size);
	auto max = (v2)(aabb.max / cell_size);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			AabbCell cell = {x, y};
			if (!cells.has(cell))
			{
				cells.insert(cell, Array<AabbGridNode<T>>());
			}

			cells.get({x, y}).add({next_id, udata});
		}
	}
}

template <typename T>
void AabbGrid<T>::query(Cute::Aabb aabb, Func<bool, T> fn)
{
	query_cache.clear();

	using namespace Cute;

	auto min = (v2)(aabb.min / cell_size);
	auto max = (v2)(aabb.max / cell_size);

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			for (AabbGridNode<T> &node : cells.get({x, y}))
			{
				if (arr_contains(
						query_cache.begin(),
						query_cache.end(),
						node.id
					))
				{
					return;
				}

				query_cache.add(node.id);

				if (!fn(node.udata))
				{
					return;
				}
			}
		}
	}
}
