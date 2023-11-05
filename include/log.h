#pragma once

#include <fmt/format.h>

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>

template <typename T, size_t S>
inline constexpr size_t get_file_name_offset(const T (&str)[S],
											 size_t i = S - 1)
{
	return (str[i] == '/' || str[i] == '\\')
				   ? i + 1
				   : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
}

template <typename T>
inline constexpr size_t get_file_name_offset(T (&str)[1])
{
	return 0;
}

namespace log_utility
{
	template <typename T, T v>
	struct const_expr_value {
		static constexpr const T value = v;
	};
}

#define UTILITY_CONST_EXPR_VALUE(exp)                                          \
	::log_utility::const_expr_value<decltype(exp), exp>::value

#define LOG_FILENAME (__FILE__ + get_file_name_offset(__FILE__))
#define log(val, lvl, ...)                                                     \
	fmt::print("{} {} {:<16} {}\n",                                            \
			   fmt::format(fmt::fg(fmt::terminal_color::white),                \
						   "{:%H:%M:%S}",                                      \
						   fmt::localtime(std::time(nullptr))),                \
			   lvl,                                                            \
			   fmt::format("{}:{}", LOG_FILENAME, __LINE__),                   \
			   fmt::format(val, ##__VA_ARGS__))

#define log_trace(val, ...)                                                    \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::white), "{:5}", "trace"),     \
		##__VA_ARGS__)

#define log_info(val, ...)                                                     \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::cyan), "{:5}", "info"),       \
		##__VA_ARGS__)

#define log_debug(val, ...)                                                    \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::green), "{:5}", "debug"),     \
		##__VA_ARGS__)

#define log_warning(val, ...)                                                  \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::yellow), "{:5}", "warn"),     \
		##__VA_ARGS__)

#define log_error(val, ...)                                                    \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::red), "{:5}", "error"),       \
		##__VA_ARGS__)

#define log_fatal(val, ...)                                                    \
	log(val,                                                                   \
		fmt::format(fmt::fg(fmt::terminal_color::black) |                      \
							fmt::bg(fmt::terminal_color::red),                 \
					"{:5}",                                                    \
					"fatal"),                                                  \
		##__VA_ARGS__)
