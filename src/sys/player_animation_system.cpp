#include "sys/player_animation_system.h"
#include "cmp/input.h"
#include "cmp/player.h"
#include "cmp/sprite.h"

void player_animation_system(World &world, float dt)
{
	static const char *anims[4] =
		{"idle-left", "idle-right", "walk-left", "walk-right"};

	world.view<Player, Input, Sprite>().each(
		[&](auto e, Player &p, Input &i, Sprite &s) {
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
