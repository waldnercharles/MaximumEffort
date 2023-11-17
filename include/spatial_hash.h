#pragma once
#include "array_algorithms.h"
#include "common.h"
#include "geometry.h"
#include <glm/glm.hpp>

#include <cute.h>

template <typename T>
struct SpatialHashNode
{
	size_t id;
	T value;
};

template <typename T>
struct SpatialHash
{
	SpatialHash();
	SpatialHash(f32 cell_size);

	void add(Aabb aabb, T value);
	void query(Aabb aabb, Func<bool, T> fn);
	void clear();

private:
	int cell_size;
	Cute::Map<glm::ivec2, Cute::Array<SpatialHashNode<T>>> cells;

	size_t next_id;
	Cute::Array<size_t> query_cache;
};

template <typename T>
SpatialHash<T>::SpatialHash() : SpatialHash(16)
{
}

template <typename T>
SpatialHash<T>::SpatialHash(f32 cell_size) : cell_size(cell_size),
											 next_id(0)
{
}

template <typename T>
void SpatialHash<T>::add(Aabb aabb, T value)
{
	++next_id;

	Cute::v2 min = cf_div_v2_f(aabb.min, cell_size);
	Cute::v2 max = cf_div_v2_f(aabb.max, cell_size);

	for (int x = floor(min.x); x <= ceil(max.x); x++)
	{
		for (int y = floor(min.y); y <= ceil(max.y); y++)
		{
			Cute::Array<SpatialHashNode<T>> *bucket = cells.try_get({x, y});
			if (!bucket)
			{
				bucket = cells.insert(
					{x, y},
					Cute::Array<SpatialHashNode<T>>()
				);
			}

			bucket->add({next_id, value});
		}
	}
}

template <typename T>
void SpatialHash<T>::query(Aabb aabb, Func<bool, T> fn)
{
	query_cache.clear();

	Cute::v2 min = cf_div_v2_f(aabb.min, cell_size);
	Cute::v2 max = cf_div_v2_f(aabb.max, cell_size);

	for (int x = floor(min.x); x <= ceil(max.x); x++)
	{
		for (int y = floor(min.y); y <= ceil(max.y); y++)
		{
			for (SpatialHashNode<T> &node : cells.get({x, y}))
			{
				if (arr_contains(
						query_cache.begin(),
						query_cache.end(),
						node.id
					))
				{
					continue;
				}


				query_cache.add(node.id);
				if (!fn(node.value))
				{
					return;
				}
			}
		}
	}
}

template <typename T>
void SpatialHash<T>::clear()
{
	for (int i = 0; i < cells.count(); i++)
	{
		cells.vals()[i].clear();
	}
	next_id = 0;
}
