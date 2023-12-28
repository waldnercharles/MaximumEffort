#include <pico_unit.h>

#include "events/event_bus.h"

struct Event
{
	int x, y, z;
};

TEST_CASE(test_event)
{
	EventBus event_bus;
	bool fired;
	int sum;

	auto subscription = event_bus.on<Event>([&](Event event) {
		fired = true;
		sum = event.x + event.y + event.z;
	});
	REQUIRE(event_bus.count() > 0);

	{
		fired = false;
		sum = 0;

		event_bus.emit<Event>({1, 2, 3});
		REQUIRE(fired);
		REQUIRE(sum == 6);
	}

	{
		event_bus.off(subscription);
		REQUIRE(event_bus.count() == 0);
	}

	{
		fired = false;
		sum = 0;
		event_bus.emit<Event>({1, 2, 3});
		REQUIRE(!fired);
		REQUIRE(sum == 0);
	}

	return true;
}

TEST_SUITE(test_event_system)
{
	RUN_TEST_CASE(test_event);
}