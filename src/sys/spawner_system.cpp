#include "sys/spawner_system.h"
#include "cmp/enemy_component.h"
#include "cmp/movement_component.h"
#include "cmp/player_component.h"
#include "cmp/spawner_component.h"
#include "factories.h"
#include "game.h"

#include <cute.h>

using namespace Cute;

Aabb get_max_enemy_bounds(v2 center_pos)
{
	return make_aabb_center_half_extents(center_pos, game.camera_size * 0.55f);
}

Cute::v2 get_enemy_spawn_pos(v2 center_pos)
{
	float angle = rnd_next_range(game.rnd, -CF_PI, CF_PI);
	v2 dir = V2(cosf(angle), sinf(angle));

	Aabb bounds = get_max_enemy_bounds(center_pos);
	return clamp(bounds, center(bounds) + dir * cf_len(half_extents(bounds)));
}

void spawner_system(entt::registry &reg, float dt)
{
	auto player = reg.view<PlayerComponent, MovementComponent>().front();
	auto *player_movement = reg.try_get<MovementComponent>(player);

	if (player_movement == nullptr)
	{
		return;
	}

	auto enemies = reg.view<EnemyComponent, MovementComponent>();
	for (auto e : enemies)
	{
		auto &m = enemies.get<MovementComponent>(e);

		Aabb bounds = get_max_enemy_bounds(player_movement->pos);
		if (!contains(bounds, m.pos))
		{
			m.pos = get_enemy_spawn_pos(player_movement->pos);
		}
	}

	auto spawners = reg.view<SpawnerComponent>();
	for (auto e : spawners)
	{
		auto &s = spawners.get<SpawnerComponent>(e);

		if (on_interval(s.rate))
		{
			switch (s.entity_type)
			{
				case ENEMY_EYEBALL:
				{
					make_enemy_eyeball(
						game.reg,
						get_enemy_spawn_pos(player_movement->pos)
					);
					break;
				}
			}
		}
	}
}
