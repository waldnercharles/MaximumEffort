#include "sys/player_animation_system.h"
#include "cmp/input_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"

void PlayerAnimationSystem::update(World &world)
{
	static const char *anims[4] =
		{"idle-left", "idle-right", "walk-left", "walk-right"};

	world.view<PlayerComponent, InputComponent, SpriteComponent>().each(
		[](auto e, PlayerComponent &p, InputComponent &i, SpriteComponent &s) {
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
