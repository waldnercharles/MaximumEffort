#include "sys/animation_system.h"
#include "cmp/age_component.h"
#include "cmp/enemy_component.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

void handle_player_animation(World &world)
{
	world.view<C_Player, C_Input, C_Sprite, C_Facing>().each(
		[](auto e, C_Input &i, C_Sprite &s, C_Facing &f) {
			const char *anim =
				i.input_dir.x != 0 || i.input_dir.y != 0 ? "walk" : "idle";

			if (!s.is_playing(anim))
			{
				s.play(anim);
			}

			if (cf_sign(s.scale.x) == cf_sign(f.facing_x))
			{
				s.flip_x();
			}
		}
	);
}

void handle_enemy_animation(World &world)
{
	// Wiggle
	world.view<C_Enemy, C_Sprite, C_Transform, C_Age>().each(
		[](auto e, C_Sprite &s, C_Transform &t, C_Age &a) {
			auto angle = sin((CF_SECONDS - a.alive_since) * 6.f) * 0.066f;
			t.set_rotation(angle);
		}
	);

	world.view<C_Enemy, C_Sprite, C_Facing>().each([](C_Sprite &s,
													  C_Facing &f) {
		if (!s.is_playing("walk") && s.animations)
		{
			if (hfind(s.animations, cf_sintern("walk")) != nullptr)
			{
				cf_sprite_reset(&s);
				s.play("walk");
			}
		}

		if (cf_sign(s.scale.x) == cf_sign(f.facing_x))
		{
			s.flip_x();
		}
	});
}

void AnimationSystem::update(World &world)
{
	handle_player_animation(world);
	handle_enemy_animation(world);
}
