#include <charconv>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

#include "doctest.h"

#include "day02.h"

using namespace std::literals;

struct CubeSet {
    int red{0};
    int green{0};
    int blue{0};

    bool operator==(const CubeSet &) const = default;
};

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

CubeSet parse_draw(std::string_view draw) {
    CubeSet set{};
    struct {
        int &field;
        std::string name;
        bool is_set;
    } field_mappings[] = {
        {set.red, " red", false},
        {set.green, " green", false},
        {set.blue, " blue", false},
    };

    auto part_begin = draw.begin();
    auto part_end = draw.begin();
    while (part_begin != draw.end()) {
        part_end = std::find(part_end, draw.end(), ',');
        std::string_view draw_part{part_begin, part_end};

        for (auto &fm : field_mappings) {
            if (draw_part.ends_with(fm.name)) {
                if (fm.is_set) {
                    throw std::invalid_argument{
                        std::format("Repeated color{}", fm.name)};
                }
                size_t number_end = draw_part.size() - fm.name.size();
                auto parse_result = std::from_chars(
                    &draw_part[1], &draw_part[number_end], fm.field);
                if (parse_result.ec != std::errc{} ||
                    parse_result.ptr != &draw_part[number_end]) {
                    throw std::invalid_argument{std::format(
                        "Invalid dice number for color{} in part", fm.name)};
                }
                fm.is_set = true;
            }
        }

        if (part_end != draw.end())
            part_end++; // skip the separator on every iteration except last
        part_begin = part_end;
    }

    return set;
}

TEST_CASE("parsing single draw") {
    CHECK_EQ(parse_draw(""), CubeSet{});
    CHECK_EQ(parse_draw(" 1 red"), CubeSet{1, 0, 0});
    CHECK_EQ(parse_draw(" 1 red, 2 green, 3 blue"), CubeSet{1, 2, 3});
    CHECK_THROWS(parse_draw(" 1 red, 2 green, 3 red"));
}