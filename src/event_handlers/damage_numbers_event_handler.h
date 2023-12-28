#pragma once
#include "cmp/transform_component.h"
#include "common.h"
#include "events/damage_event.h"
#include "events/event_bus.h"

struct DamageNumberParticle
{
	char text[12];

	v2 pos;
	v2 vel;
	float alpha;
	float lifetime;

	float scale;
};

struct DamageNumbersEventHandler
{
	DamageNumbersEventHandler(World &world, EventBus &event_bus);
	~DamageNumbersEventHandler();

	// TODO: This should be moved into its own system
	void update();
	void draw();

	void add(v2 pos, int damage_number);

private:
	EventBus &event_bus;

	const float lifetime;
	Array<DamageNumberParticle> particles;
	Subscription subscription;
};
