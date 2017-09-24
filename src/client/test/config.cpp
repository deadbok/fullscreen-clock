#include "../config.h"
#include "catch.hpp"

TEST_CASE("Testing configuration parser", "[config_parse]")
{
    Config config;

    // Test a comment line.
    config.clear();
    REQUIRE(config.parse_line(
                "# The following lines should parse without error.", 0) == 0);
    REQUIRE(config.size() == 0);

    // Another comment with a tab at the beginning.
    config.clear();
    REQUIRE(config.parse_line(
                "\t# The following lines should parse without error.", 0) == 0);
    REQUIRE(config.size() == 0);

    // Another comment with a space in the beginning.
    config.clear();
    REQUIRE(config.parse_line(
                " # The following lines should parse without error.", 0) == 0);
    REQUIRE(config.size() == 0);

    // A syntactically correct configuration line.
    config.clear();
    REQUIRE(config.parse_line("owm_api_key = \"42\"", 0) == 0);
    REQUIRE(config.size() == 1);
    REQUIRE(config.get_value("owm_api_key") == "42");

    // Empty line.
    config.clear();
    REQUIRE(config.parse_line("", 0) == 0);
    REQUIRE(config.size() == 0);

    // Correct configuration line with no spaces and ending in a comment.
    config.clear();
    REQUIRE(config.parse_line("fullscreen=yes #We want fullscreen.", 0) == 0);
    REQUIRE(config.size() == 1);
    REQUIRE(config.get_value("fullscreen") == "yes");

    // Comment identifier in the value is still part of the value.
    config.clear();
    REQUIRE(config.parse_line("test = \"#test\"", 0) == 0);
    REQUIRE(config.size() == 1);
    REQUIRE(config.get_value("test") == "#test");

    // Only the configuration key, this should fail.
    config.clear();
    REQUIRE(config.parse_line("test", 0) == 1);
    REQUIRE(config.size() == 0);

    // No value, should fail.
    config.clear();
    REQUIRE(config.parse_line("no = #nono", 0) == 1);
    REQUIRE(config.size() == 0);

    // Wrong (DM).
    config.clear();
    REQUIRE(config.parse_line(" = ", 0) == 1);
    REQUIRE(config.size() == 0);

    // #Wrong (DM).
    config.clear();
    REQUIRE(config.parse_line("=test", 0) == 1);
    REQUIRE(config.size() == 0);

    // #Wrong (DM).
    config.clear();
    REQUIRE(config.parse_line("te#st = test", 0) == 1);
    REQUIRE(config.size() == 0);

    // Try parsing multiple lines.
    REQUIRE(config.parse(" # The following lines should parse without error.\n\
                 owm_api_key = \"42\"\n\n\n\t fullscreen=yes #We want \
                 fullscreen.") == 0);
    REQUIRE(config.size() == 2);
    REQUIRE(config.get_value("owm_api_key") == "42");
    REQUIRE(config.get_value("fullscreen") == "yes");

    // Try parsing a file
    REQUIRE(config.load("test-config.conf") == 0);
    REQUIRE(config.size() == 4);
    REQUIRE(config.get_value("owm_api_key") == "42");
    REQUIRE(config.get_value("owm_city_id") == "2615876");
    REQUIRE(config.get_value("test") == "#test");
    REQUIRE(config.get_value("fullscreen") == "yes");
}
