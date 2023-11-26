#include "sys/player_animation_system.h"
#include "cmp/enemy_component.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"

void AnimationSystem::update(World &world)
{
	static const char *anims[4] =
		{"idle-left", "idle-right", "walk-left", "walk-right"};

	world
		.view<
			PlayerComponent,
			const InputComponent,
			SpriteComponent,
			const FacingComponent>()
		.each([](auto e,
				 const InputComponent &i,
				 SpriteComponent &s,
				 const FacingComponent &f) {
			int action_anim = i.input_dir.x != 0 || i.input_dir.y != 0 ? 2 : 0;
			int facing_anim = f.facing == Facing::RIGHT ? 1 : 0;

			int anim_index = action_anim + facing_anim;

			const char *anim = anims[anim_index];

			if (!s.is_playing(anim))
			{
				s.play(anim);
			}
		});

	static const char *enemy_anims[5] =
		{"walk-down", "walk-up", "walk-down", "walk-left", "walk-right"};

	world.view<EnemyComponent, SpriteComponent, const FacingComponent>().each(
		[](auto e, SpriteComponent &s, const FacingComponent &f) {
			const char *anim = enemy_anims[f.facing];

			if (!s.is_playing(anim))
			{
				s.play(anim);
			}
		}
	);
}
