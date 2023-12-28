#pragma once

#include <functional>
template <class R, class... Args>
using Func [[maybe_unused]] = std::function<R(Args...)>;

#include <cstdint>
using s8 [[maybe_unused]] = int8_t;
using s16 [[maybe_unused]] = int16_t;
using s32 [[maybe_unused]] = int32_t;
using s64 [[maybe_unused]] = int64_t;
using u8 [[maybe_unused]] = uint8_t;
using u16 [[maybe_unused]] = uint16_t;
using u32 [[maybe_unused]] = uint32_t;
using u64 [[maybe_unused]] = uint64_t;
using f32 [[maybe_unused]] = float;
using f64 [[maybe_unused]] = double;
using size_t [[maybe_unused]] = std::size_t;

#include <cute.h>
template <typename T>
using Array [[maybe_unused]] = Cute::Array<T>;

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

template <typename K, typename T>
using Map [[maybe_unused]] = Cute::Map<K, T>;

#include <unordered_set>
template <
	class K,
	class HashFn = std::hash<K>,
	class EqFn = std::equal_to<K>,
	class Alloc = std::allocator<K>>
using Set [[maybe_unused]] = std::unordered_set<K, HashFn, EqFn, Alloc>;

using String [[maybe_unused]] = Cute::String;
using Sprite [[maybe_unused]] = Cute::Sprite;

#define PI 3.14159265f

// clang-format off
using v2 [[maybe_unused]] = CF_V2;
CF_INLINE v2 operator+(v2 a, v2 b) { return cf_v2(a.x + b.x, a.y + b.y); }
CF_INLINE v2 operator-(v2 a, v2 b) { return cf_v2(a.x - b.x, a.y - b.y); }
CF_INLINE v2& operator+=(v2& a, v2 b) { return a = a + b; }
CF_INLINE v2& operator-=(v2& a, v2 b) { return a = a - b; }

CF_INLINE v2 operator*(v2 a, float b) { return cf_v2(a.x * b, a.y * b); }
CF_INLINE v2 operator*(v2 a, v2 b) { return cf_v2(a.x * b.x, a.y * b.y); }
CF_INLINE v2& operator*=(v2& a, float b) { return a = a * b; }
CF_INLINE v2& operator*=(v2& a, v2 b) { return a = a * b; }
CF_INLINE v2 operator/(v2 a, float b) { return cf_v2(a.x / b, a.y / b); }
CF_INLINE v2 operator/(v2 a, v2 b) { return cf_v2(a.x / b.x, a.y / b.y); }
CF_INLINE v2& operator/=(v2& a, float b) { return a = a / b; }
CF_INLINE v2& operator/=(v2& a, v2 b) { return a = a / b; }

CF_INLINE v2 operator-(v2 a) { return cf_v2(-a.x, -a.y); }

CF_INLINE int operator<(v2 a, v2 b) { return a.x < b.x && a.y < b.y; }
CF_INLINE int operator>(v2 a, v2 b) { return a.x > b.x && a.y > b.y; }
CF_INLINE int operator<=(v2 a, v2 b) { return a.x <= b.x && a.y <= b.y; }
CF_INLINE int operator>=(v2 a, v2 b) { return a.x >= b.x && a.y >= b.y; }
// clang-format on

using SinCos = CF_SinCos;
using M2x2 = CF_M2x2;
using M3x2 = CF_M3x2;
//using Transform = CF_Transform;
using Halfspace = CF_Halfspace;
using Ray = CF_Ray;
using Raycast = CF_Raycast;
using Circle = CF_Circle;
using Aabb = CF_Aabb;
using Rect = CF_Rect;
using Poly = CF_Poly;
using SliceOutput = CF_SliceOutput;
using Capsule = CF_Capsule;
using Manifold = CF_Manifold;
using GjkCache = CF_GjkCache;
using ToiResult = CF_ToiResult;
using ShapeType = CF_ShapeType;
using Rnd = CF_Rnd;
using Pixel = CF_Pixel;
using Color = CF_Color;
using Png = Cute::Png;
using Path = Cute::Path;

#include <entt/entt.hpp>
using Entity = entt::entity;
using World = entt::registry;
inline constexpr entt::null_t ECS_NULL {};


#ifdef _MSC_VER
#pragma warning(disable : 4291)
#endif
enum DummyEnum
{
	DUMMY_ENUM
};
inline void *operator new(size_t, DummyEnum, void *ptr)
{
	return ptr;
}
#define PLACEMENT_NEW(ptr) new (DUMMY_ENUM, ptr)
#define NEW(...)           new (DUMMY_ENUM, malloc(sizeof(__VA_ARGS__)))(__VA_ARGS__)