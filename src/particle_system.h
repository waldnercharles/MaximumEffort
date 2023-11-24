#pragma once
#include "cmp/transform_component.h"
#include "common.h"
#include "event.h"
#include "events/damage_event.h"
#include <charconv>

struct DamageNumberParticle
{
	char text[12];

	v2 pos;
	v2 vel;
	float alpha;
	float lifetime;

	float scale;
};

struct DamageNumbers
{
	DamageNumbers(World &world, EventBus &event_bus)
		: event_bus(event_bus),
		  rnd(cf_rnd_seed(time(nullptr))),
		  lifetime(0.66f)
	{
		subscription = event_bus.on<DamageEvent>([&](DamageEvent e) {
			auto pos = world.get<TransformComponent>(e.entity)
						   .get_world_transform()
						   .pos;

			add(pos, e.damage);
		});
	}

	~DamageNumbers()
	{
		event_bus.off(subscription);
	}

	void add(v2 pos, int damage_number)
	{
		DamageNumberParticle p = {};
		std::to_chars(p.text, p.text + 12, damage_number);
		p.pos = pos;
		p.alpha = 10.f;
		p.scale = 1.f;

		float force = 100.f;
		p.vel = {
			cf_rnd_next_range_float(&rnd, -force, force),
			cf_rnd_next_range_float(&rnd, -force, force)};

		p.lifetime = lifetime;

		particles.add(p);
	}

	void update()
	{
		for (int i = particles.size() - 1; i >= 0; i--)
		{
			auto &p = particles[i];
			p.vel *= 0.95f;
			p.pos += p.vel * CF_DELTA_TIME_FIXED;

			p.lifetime -= CF_DELTA_TIME_FIXED;
			if (p.lifetime <= 0)
			{
				particles.unordered_remove(i);
			}
		}
	}

	void draw()
	{
		cf_draw_push_layer(4096);
		cf_push_font("ProggyClean");
		const float font_size = 13.f;
		for (auto p : particles)
		{
			cf_push_font_size(font_size * p.scale);
			cf_push_font_blur(1);
			cf_draw_push_color({0.f, 0.f, 0.f, 1.f});
			cf_draw_text(p.text, p.pos, -1);
			cf_draw_pop_color();
			cf_pop_font_blur();
			cf_pop_font_size();

			cf_push_font_size(font_size * p.scale);
			cf_draw_push_color({1.f, 1.f, 1.f, 1.f});
			cf_draw_text(p.text, p.pos, -1);
			cf_draw_pop_color();
			cf_pop_font_size();
		}
		cf_pop_font();
		cf_draw_pop_layer();
	}

private:
	EventBus &event_bus;

	Rnd rnd;
	const float lifetime;

	Array<DamageNumberParticle> particles;

	subscription_id subscription;
};
