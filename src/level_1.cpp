#include "level_1.h"

#include "assets.h"
#include "cmp/player_component.h"
#include "log.h"
#include "prefabs/enemy_spawner_prefab.h"

void make_level_1(Game &game)
{
	auto player = game.world.view<PlayerComponent>().front();

	auto csv = parse_csv_file("waves.csv");
	for (int i = 1; i < csv.count(); i++)
	{
		auto &row = csv[i];
		log_info("Parsed wave {}", row[0].c_str());

		int start = row[1].to_int();
		int end = row[2].to_int();
		const char *type = sintern(row[3]);
		float interval = row[4].to_float();
		int spawns_per_interval = row[5].to_int();
		int max_spawns = row[6].to_int();
		bool spawn_once = row[7].to_bool();

		prefabs::EnemySpawner::create(
			game.world,
			type,
			start,
			end,
			interval,
			spawns_per_interval,
			max_spawns,
			spawn_once,
			player
		);
	}
}