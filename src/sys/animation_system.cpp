#include "sys/animation_system.h"
#include "cmp/age_component.h"
#include "cmp/enemy_component.h"
#include "cmp/facing_component.h"
#include "cmp/input_component.h"
#include "cmp/player_component.h"
#include "cmp/sprite_component.h"
#include "cmp/transform_component.h"

void AnimationSystem::update(World &world)
{
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
		});

	world
		.view<
			EnemyComponent,
			SpriteComponent,
			TransformComponent,
			AgeComponent>()
		.each([](auto e,
				 SpriteComponent &s,
				 TransformComponent &t,
				 AgeComponent &a) {
			// Wiggle
			auto angle = sin((CF_SECONDS - a.alive_since) * 6.f) * 0.066f;

			t.set_rotation(angle);
		});

	world.view<EnemyComponent, SpriteComponent, FacingComponent>().each(
		[](SpriteComponent &s, FacingComponent &f) {
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
		}
	);
}
