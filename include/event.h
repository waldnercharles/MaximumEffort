#pragma once

#include "common.h"
#include "type_id.h"

#include <functional>
#include <numeric>
#include <stack>
#include <vector>

enum SubscriptionType : u8
{
	Forever,
	Once
};

struct subscription_id
{
	int type_id;
	int id;
};

inline static bool
operator==(const subscription_id &a, const subscription_id &b)
{
	return a.type_id == b.type_id && a.id == b.id;
}

struct Subscription
{
	subscription_id id;
	Func<void, void *> fn;

	bool once;
};

struct EventBus
{
	template <typename EventType, typename CallbackFn>
	subscription_id on(CallbackFn fn)
	{
		const int index = type_id<EventType>();
		subscriptions.ensure_count(index + 1);

		int id;
		if (free_list.size())
		{
			id = free_list.top();
			free_list.pop();
		}
		else
		{
			id = ++next_id;
		}

		auto fn_wrapper = [fn](void *event) {
			fn(*(EventType *)event);
		};
		subscriptions[index].add({{index, id}, fn_wrapper, false});

		return {index, id};
	}

	void off(subscription_id s)
	{
		for (int i = 0; i < subscriptions[s.type_id].size(); i++)
		{
			if (subscriptions[s.type_id][i].id == s)
			{
				subscriptions[s.type_id].unordered_remove(i);
				free_list.push(s.id);
				return;
			}
		}
	}

	template <class EventType, typename... Args>
	void emit(Args... args)
	{
		EventType event {std::forward<Args>(args)...};
		emit_(event);
	}

	template <typename T, typename... Args>
	void emit(T &&event)
	{
		emit_(event);
	}

	int count() const
	{
		int count = 0;
		for (auto subs : subscriptions)
		{
			count += subs.size();
		}

		return count;
	}

	void clear()
	{
		for (int i = 0; i < subscriptions.size(); i++)
		{
			subscriptions[i].clear();
		}
	}

private:
	template <typename EventType>
	void emit_(EventType &event)
	{
		emit_(type_id<EventType>(), &event);
	}

	void emit_(int id, void *event)
	{
		if ((int)id < subscriptions.size())
		{
			for (auto sub : subscriptions[id])
			{
				sub.fn(event);
			}
		}
	}

	Array<Array<Subscription>> subscriptions;

	int next_id = 0;
	std::stack<int> free_list;
};