#include <stdexcept>
#include <string>
#include <string_view>

#include "doctest.h"

#include "day02.h"

using namespace std::literals;

int parse_game_id(std::string_view line) {
    if (!line.starts_with("Game "sv)) {
        throw std::invalid_argument{"Expected line to start with 'Game '"};
    }
    return 0;
}

TEST_CASE("parsing game id") {
    CHECK_THROWS(parse_game_id("Gaym 15: 3 blue, 4 red"));
}