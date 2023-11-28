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
	DamageNumbers(World &world, EventBus &event_bus);

	~DamageNumbers();


	void update();

	void draw();

	void add(v2 pos, int damage_number);

private:
	EventBus &event_bus;

	const float lifetime;

	Array<DamageNumberParticle> particles;

	subscription_id subscription;
};
