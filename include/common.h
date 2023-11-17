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

//#include <cute.h>
//template <typename K, typename T>
//using Map [[maybe_unused]] = Cute::Map<K, T>;
//
//template <typename T>
//using Array [[maybe_unused]] = Cute::Array<T>;

#ifdef CF_CPP
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
#endif