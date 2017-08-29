#include "../config.h"
#include "catch.hpp"

TEST_CASE("Testing configuration parser", "[config_parse]")
{
    Config config;

    REQUIRE(config.parse("# The following lines should parse without error.",
                         0) == 0);
    REQUIRE(config.parse("  # The following lines should parse without error.",
                         0) == 0);
    REQUIRE(config.parse("owm_api_key = \"42\"", 0) == 0);
    REQUIRE(config.parse("", 0) == 0);
    REQUIRE(config.parse("fullscreen=yes #We want fullscreen.", 0) == 0);

    REQUIRE(config.parse("wtf", 0) == 1);
}
