#pragma once
#include "cmp/stats_component.h"
#include <unordered_set>

using PassiveId = const char *;

struct Passive
{
	PassiveId id = {};
	const char *name = nullptr;
	bool is_root = false;

	Array<const char *> description;
	StatsModifier stats_modifier;

	inline bool operator==(const Passive &other)
	{
		return id == other.id;
	}
};

struct C_PassiveTree
{
	void load();
	void update_available_passives();

	void unlock_passive(const Passive &passive);

	inline const Array<Passive> get_available_passives()
	{
		return available_passives;
	}


private:
	Map<PassiveId, Passive> passives;
	Map<PassiveId, Set<PassiveId>> edges;

	Set<PassiveId> unlocked_passives;
	Array<Passive> available_passives;
};
