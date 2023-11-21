#pragma once
#include <cstdint>

template <typename T>
inline static uint32_t type_id()
{
	return type_id_builder::get<T>();
}

struct type_id_builder
{
	template <typename T>
	inline static uint32_t get()
	{
		return sanitized<std::remove_cv_t<cf_remove_reference<T>>>();
	}

private:
	inline static uint32_t identifier()
	{
		static uint32_t value = 0;
		return value++;
	}

	template <typename T>
	inline static uint32_t sanitized()
	{
		static const uint32_t value = identifier();
		return value;
	}
};
