#include "sys/enemy_target_system.h"
#include "cmp/follow_target_behavior_component.h"
#include "cmp/movement_component.h"
#include "cmp/scene_node_component.h"

using namespace Cute;

void enemy_target_system(entt::registry &reg, float dt)
{
	reg.view<SceneNodeComponent, FollowTargetBehaviorComponent>().each(
		[&](auto e, SceneNodeComponent &s, FollowTargetBehaviorComponent &t) {
			auto pos = s.get_global_transform().pos;

			auto *other = reg.try_get<SceneNodeComponent>(t.entity);

			if (other)
			{
				auto other_pos = other->get_global_transform().pos;
				t.dir = safe_norm(other_pos - pos);

				if (t.face_target)
				{
					s.set_rotation(atan2(t.dir.y, t.dir.x));
				}
			}
		}
	);
}
