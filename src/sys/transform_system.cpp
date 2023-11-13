#include "cmp/c_transform.h"
#include <flecs.h>

using namespace Cute;

void calculate_world_transform(
	const C_LocalTransform &local,
	const C_WorldTransform *parent,
	C_WorldTransform &out
)
{
	out.pos = local.pos;
	out.angle = local.angle;
	if (parent)
	{
		out.pos += parent->pos;
	}
}

void add_transform_system(flecs::world *w)
{
	auto s = w->system<
		const C_LocalTransform,
		const C_WorldTransform *,
		C_WorldTransform>();

	s.term_at(2).parent().cascade().optional();
	s.each(calculate_world_transform);
}