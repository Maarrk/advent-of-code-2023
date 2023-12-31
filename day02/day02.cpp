#include <algorithm>
#include <charconv>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

#include "doctest.h"

#include "day02.h"

using namespace std::literals;

const CubeSet CubeSet::valid_game_limit{12, 13, 14};

int parse_game_id(const std::string_view line) {
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

CubeSet parse_draw(const std::string_view draw) {
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
                        "Invalid dice number for color{} in part '{}'", fm.name,
                        draw_part)};
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

bool CubeSet::is_subset(const CubeSet &other) const {
    return (red <= other.red) && (green <= other.green) && (blue <= other.blue);
}

CubeSet CubeSet::set_union(const CubeSet &other) const {
    return CubeSet{
        .red = std::max(red, other.red),
        .green = std::max(green, other.green),
        .blue = std::max(blue, other.blue),
    };
}

TEST_CASE("set operations") {
    CHECK_EQ(CubeSet{}.set_union(CubeSet{}), CubeSet{});
    CHECK_EQ(CubeSet{1, 0, 0}.set_union(CubeSet{0, 1, 0}), CubeSet{1, 1, 0});
    CHECK_EQ(CubeSet{1, 0, 1}.set_union(CubeSet{0, 0, 2}), CubeSet{1, 0, 2});
}

int CubeSet::power() const { return red * green * blue; }

CubeSet game_required_set(const std::string_view line) {
    auto draw_begin = std::find(line.begin(), line.end(), ':');
    if (draw_begin == line.end()) {
        throw std::invalid_argument{"Expected line to have ':' character"};
    }
    auto draw_end = ++draw_begin; // skip the separator

    CubeSet required{};
    while (draw_begin != line.end()) {
        draw_end = std::find(draw_end, line.end(), ';');
        std::string_view draw{draw_begin, draw_end};
        required = required.set_union(parse_draw(draw));

        if (draw_end != line.end())
            draw_end++; // skip the separator on every iteration except last
        draw_begin = draw_end;
    }

    return required;
}

TEST_CASE("validating a game") {
    CHECK(game_required_set("Game 1: 1 blue, 2 green, 3 red")
              .is_subset(CubeSet::valid_game_limit));
    CHECK_THROWS(game_required_set("Game 1 2 blue"));
    CHECK(game_required_set(
              "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green")
              .is_subset(CubeSet::valid_game_limit));
    CHECK_FALSE(
        game_required_set("Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, "
                          "13 green; 5 green, 1 red")
            .is_subset(CubeSet::valid_game_limit));
}
