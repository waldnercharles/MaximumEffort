#include "sys/player_animation_system.h"

#include "cmp/c_input.h"
#include "cmp/c_player.h"
#include "cmp/c_sprite.h"

#include <flecs.h>

using namespace Cute;

void add_player_animation_system(flecs::world *world)
{
	world->system<const C_Player, const C_Input, C_Sprite>("player_animation")
		.each([](const C_Player &p, const C_Input &i, C_Sprite &s) {
			static const char *anims[4] =
				{"idle-left", "idle-right", "walk-left", "walk-right"};

			int action_anim = i.input_dir.x != 0 || i.input_dir.y != 0 ? 2 : 0;
			int facing_anim = p.facing > 0 ? 1 : 0;

			int anim_index = action_anim + facing_anim;

			const char *anim = anims[anim_index];

			if (!s.sprite.is_playing(anim))
			{
				s.sprite.play(anim);
			}
		});
}
