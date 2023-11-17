#pragma once

struct _
{
	template <typename T>
	static T &get()
	{
		static T instance;
		return instance;
	}
};
