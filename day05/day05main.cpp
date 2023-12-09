#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "day05.h"

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

    auto sol = solution(input_file, input_filename.ends_with("example.txt"));
    std::cout << "Part 1 answer: " << sol.first << std::endl
              << "Part 2 answer: " << sol.second << std::endl;

    return EXIT_SUCCESS;
}