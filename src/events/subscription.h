#pragma once
#include "common.h"

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
};
