#include "assets.h"
#include <pico_unit.h>

TEST_CASE(test_csv_parsing)
{
	const char *csv =
		"name,health,speed,hitbox,hurtbox,scale,behavior,sprite,color\n"
		"bat,100,25,8,6,1,follow,bat.ase,0xFFFFFF\n"
		"hellhound,100,25,8,6,1,follow,boar.ase,0xFFFFFF\n"
		"hellhound_big,100,25,8,6,1,follow,boar.ase,0xDEADBEEF\n"
		"hellhound_small,100,25,8,6,1,follow,boar.ase,0xABCDEF\n"
		"hellhound_small,100,25,8,6,1,follow,boar.ase,0xABCDEF\n"
		"imp,100,25,8,6,1,follow,,0xFFFFFF";

	auto parsed_csv = parse_csv_str(csv);

	REQUIRE(parsed_csv[0][0] == "name");
	REQUIRE(parsed_csv[4][0] == "hellhound_small");
	REQUIRE(parsed_csv[3][2] == "25");
	REQUIRE(parsed_csv[3][8] == "0xDEADBEEF");

	REQUIRE(parsed_csv[4][8] == "0xABCDEF");
	REQUIRE(parsed_csv[5][8] == "0xABCDEF");
	REQUIRE(parsed_csv[6][7] == "");

	return true;
}

TEST_SUITE(test_csv)
{
	RUN_TEST_CASE(test_csv_parsing);
}
