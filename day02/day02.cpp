#include <charconv>
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

    auto it = std::find(line.begin(), line.end(), ':');
    if (it == line.end()) {
        throw std::invalid_argument{"Expected line to have ':' character"};
    }

    int id;
    auto parse_result = std::from_chars(&line[5], &(*it), id);
    if (parse_result.ec != std::errc{} || parse_result.ptr != &(*it)) {
        throw std::invalid_argument{"Invalid game id"};
    }

    return id;
}

TEST_CASE("parsing game id") {
    CHECK_THROWS(parse_game_id("Gaym 15: 3 blue, 4 red"));
    CHECK_THROWS(parse_game_id("Game 15 3 blue, 4 red"));
    CHECK_THROWS(parse_game_id("Game : 3 blue, 4 red"));
    CHECK_THROWS(parse_game_id("Game 1a: 3 blue, 4 red"));
    CHECK_EQ(parse_game_id("Game 1: 3 blue, 4 red"), 1);
    CHECK_EQ(parse_game_id("Game 15: 3 blue, 4 red"), 15);
}