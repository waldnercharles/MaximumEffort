#pragma once
#include <cute.h>

template <typename T>
T *arr_find(T *begin, T *end, T val)
{
	T *it;
	for (it = begin; it < end; it++)
	{
		if (*it == val)
		{
			break;
		}
	}

	return it;
}

template <typename T>
bool arr_contains(T *begin, T *end, T val)
{
	return arr_find(begin, end, val) != end;
}