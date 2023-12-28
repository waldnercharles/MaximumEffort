#include "passive_tree.h"
#include "assets.h"
#include "cmp/stats_component.h"
#include "log.h"
#include "serialization/json.h"
#include <regex>
#include <unordered_map>

Map<PassiveId, Passive> load_passives()
{
	static const std::unordered_map<const char *, Stat> stat_lookup {
		{sintern("Life"), Stat::health},
		{sintern("Movement Speed"), Stat::speed},
		{sintern("Attack Speed"), Stat::attack_speed},
		{sintern("Range"), Stat::attack_range},
		{sintern("Damage"), Stat::damage}};

	static const char *increased_str = sintern("increased");
	static const char *reduced_str = sintern("reduced");
	static const char *more_str = sintern("more");
	static const char *less_str = sintern("less");
	static const char *to_str = sintern("to");

	const char *passive_regex = "\\s*((?:\\+|\\-)?(?:\\d*\\.?\\d+))(%)?\\s"
								"(increased|reduced|more|less|to)\\s[a-z\\s]*(("
								"?:\\s?[A-Z][a-z]*)*)\\s*";

	Map<PassiveId, Passive> passives;

	log_trace("{}", passive_regex);

	auto regex = std::regex(passive_regex);

	auto json_doc = cf_make_json_from_file("passives/nodes.json");
	auto nodes = cf_json_get_root(json_doc);
	auto nodes_count = cf_json_get_len(nodes);

	for (auto i = 0; i < nodes_count; i++)
	{
		auto node = cf_json_array_at(nodes, i);
		auto id = sintern(cf_json_get_string(node, "id"));
		auto data = cf_json_get(node, "data");
		auto name = sintern(cf_json_get_string(data, "name"));
		auto is_root = cf_json_get_bool(data, "isRoot");

		auto *passive = passives.insert(id, {id, name, is_root}); // TODO: Name

		auto node_passives = cf_json_get(data, "passives");
		auto node_passives_count = cf_json_get_len(node_passives);
		for (auto j = 0; j < node_passives_count; j++)
		{
			auto str = cf_json_get_string(cf_json_array_at(node_passives, j));

			std::cmatch match;

			if (std::regex_match(str, match, regex))
			{
				passive->description.add(sintern(str));

				auto amount = cf_stofloat(match[1].str().c_str());
				auto is_percent = cf_string_equ(match[2].str().c_str(), "%");
				if (is_percent)
				{
					amount /= 100.f;
				}

				const char *verb = sintern(match[3].str().c_str());
				auto negate = verb == reduced_str || verb == less_str;
				if (negate)
				{
					amount = -amount;
				}

				Modifier new_modifier = {};
				if (verb == increased_str || verb == reduced_str)
				{
					new_modifier.increase = amount;
				}
				else if (verb == more_str || verb == less_str)
				{
					new_modifier.more = amount;
				}
				else if (verb == to_str)
				{
					new_modifier.flat = amount;
				}
				else
				{
					log_warning("Failed to parse passive: {}", str);
				}

				const char *subject = sintern(match[4].str().c_str());
				auto stat_iter = stat_lookup.find(subject);
				if (stat_iter != stat_lookup.end())
				{
					auto stat = stat_iter->second;
					passive->stats_modifier[stat] += new_modifier;
				}
			}
			else
			{
				log_trace("[{}] No matches found for {}", id, str);
			}
		}
	}

	return passives;
}

Map<PassiveId, std::unordered_set<PassiveId>> load_edges()
{
	Map<PassiveId, std::unordered_set<PassiveId>> edges;

	auto json_doc = cf_make_json_from_file("passives/edges.json");
	auto json_edges = cf_json_get_root(json_doc);
	auto json_edges_count = cf_json_get_len(json_edges);

	for (auto i = 0; i < json_edges_count; i++)
	{
		auto edge = cf_json_array_at(json_edges, i);
		auto a = sintern(cf_json_get_string(edge, "source"));
		auto b = sintern(cf_json_get_string(edge, "target"));

		auto *edge_list_a = edges.try_get(a);
		auto *edge_list_b = edges.try_get(b);

		if (!edge_list_a)
		{
			edge_list_a = edges.insert(a);
		}

		if (!edge_list_b)
		{
			edge_list_b = edges.insert(b);
		}

		edge_list_a->insert(b);
		edge_list_b->insert(a);
	}

	return edges;
}

void C_PassiveTree::unlock_passive(const Passive &passive)
{
	unlocked_passives.insert(passive.id);
	update_available_passives(); // TODO: This is lazy
}

void C_PassiveTree::update_available_passives()
{
	available_passives.clear();

	for (int i = 0; i < passives.count(); i++)
	{
		auto &passive = passives.items()[i];
		if (!passive.is_root)
		{
			continue;
		}

		if (unlocked_passives.find(passive.id) == unlocked_passives.end())
		{
			available_passives.add(passive);
		}
	}

	for (auto unlocked : unlocked_passives)
	{
		auto *unlockable_passives = edges.try_get(unlocked);
		if (!unlockable_passives)
		{
			continue;
		}

		for (auto passive_id : *unlockable_passives)
		{
			if (unlocked_passives.find(passive_id) == unlocked_passives.end())
			{
				available_passives.add(passives.get(passive_id));
			}
		}
	}
}

void C_PassiveTree::load()
{
	passives = load_passives();
	edges = load_edges();

	update_available_passives();
}
