#include <fstream>
#include <iostream>
#include <numeric>
#include <string>

#include "day11.h"

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

    auto by_row = load(input_file);
    auto by_col = sorted_by_col(by_row);
    expand_universe(by_row, by_col);
    auto distances = closest_distances(by_row);
    int distance_sum = std::accumulate(distances.begin(), distances.end(), 0);

    std::cout << "Part 1 answer: " << distance_sum << std::endl;
    std::cout << "Part 2 answer: " << 0 << std::endl;

    return EXIT_SUCCESS;
}