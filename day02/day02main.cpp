#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "day02.h"

using namespace std::literals;

int main(int argc, char *argv[]) {
    const auto input_filename = [&] {
        if (argc >= 2)
            return std::string{argv[1]};
        else
            return "example.txt"s;
    }();

    std::ifstream input_file{input_filename};
    if (input_file.fail()) {
        std::cerr << "Could not open " << input_filename << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    int valid_game_sum{0};
    int required_power_sum{0};
    while (std::getline(input_file, line)) {
        CubeSet required_set = game_required_set(line);

        if (required_set.is_subset(CubeSet::valid_game_limit))
            valid_game_sum += parse_game_id(line);

        required_power_sum += required_set.power();
    }

    std::cout << "Part 1 answer: " << valid_game_sum << std::endl
              << "Part 2 answer: " << required_power_sum << std::endl;

    return EXIT_SUCCESS;
}