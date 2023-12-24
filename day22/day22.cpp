#include <regex>
#include <stdexcept>
#include <string>

#include "doctest.h"
#include "raylib.h"

#include "day22.h"

Brick::Brick(rl::Vector3 v1, rl::Vector3 v2) {
    if (v1.z > v2.z) {
        start = v2;
        end = v1;
        return;
    }

    start = v1;
    end = v2;
}

Brick Brick::from_string(const std::string &input_line) {
    std::regex pattern(R"(^(\d)+,(\d)+,(\d)+~(\d)+,(\d)+,(\d)+$)",
                       std::regex_constants::ECMAScript |
                           std::regex_constants::multiline);
    std::smatch match;
    if (!std::regex_match(input_line, match, pattern)) {
        throw std::invalid_argument{"input line didn't match pattern"};
    }

    rl::Vector3 v1(std::stoi(match[1]), std::stoi(match[2]),
                   std::stoi(match[3]));
    rl::Vector3 v2(std::stoi(match[4]), std::stoi(match[5]),
                   std::stoi(match[6]));
    return Brick(v1, v2);
}

TEST_CASE("Brick deserialisation") {
    CHECK_EQ(Brick::from_string("1,0,1~1,2,1"),
             Brick(rl::Vector3(1, 0, 1), rl::Vector3(1, 2, 1)));
}

void Brick::Draw() const {
    rl::Vector3 min = start.Min(end);
    rl::Vector3 max = start.Max(end) + rl::Vector3::One();

    rl::Vector3 center = (min + max) / 2;
    rl::Vector3 size = max - min;
    center.DrawCube(size, GRAY);
    center.DrawCubeWires(size, BLACK);
}