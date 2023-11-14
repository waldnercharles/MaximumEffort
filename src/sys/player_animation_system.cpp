#include "sys/player_animation_system.h"
#include "cmp/c_input.h"
#include "cmp/c_player.h"
#include "cmp/c_sprite.h"

#include <cute.h>

using namespace Cute;

void player_animation_system(entt::registry &reg, float dt)
{
	static const char *anims[4] =
		{"idle-left", "idle-right", "walk-left", "walk-right"};

	reg.view<C_Player, C_Input, C_Sprite>().each(
		[&](auto e, C_Player &p, C_Input &i, C_Sprite &s) {
			int action_anim = i.input_dir.x != 0 || i.input_dir.y != 0 ? 2 : 0;
			int facing_anim = p.facing > 0 ? 1 : 0;

			int anim_index = action_anim + facing_anim;

			const char *anim = anims[anim_index];

			if (!s.is_playing(anim))
			{
				s.play(anim);
			}
		}
	);
}
