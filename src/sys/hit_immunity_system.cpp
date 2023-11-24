#include "sys/hit_immunity_system.h"
#include "cmp/hit_immunity_component.h"

#include <cute.h>

void HitImmunitySystem::update(World &world)
{
	world.view<HitImmunityComponent>().each(
		[](HitImmunityComponent &hit_immunity) {
			for (int i = hit_immunity.count - 1; i >= 0; i--)
			{
				hit_immunity.hits[i].remaining_frames--;
				if (hit_immunity.hits[i].remaining_frames == 0)
				{
					hit_immunity.hits[i] = cf_move(
						hit_immunity.hits[hit_immunity.count]
					);

					hit_immunity.count--;
				}
			}
		}
	);
}
