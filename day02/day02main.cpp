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
    int sum{0};
    while (std::getline(input_file, line)) {
        if (validate_game(line))
            sum += parse_game_id(line);
    }

    std::cout << "Part 1 answer: " << sum << std::endl;

    return EXIT_SUCCESS;
}