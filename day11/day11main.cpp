#include <array>
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
    std::vector<int64_t> sums{};
    for (auto factor : std::array<int64_t, 2>{2, 1'000'000}) {
        auto expanded{by_row};
        expand_universe(expanded, by_col, factor);
        auto distances = closest_distances(expanded);
        long long distance_sum =
            std::accumulate(distances.begin(), distances.end(), 0LL);
        sums.push_back(distance_sum);
    }

    std::cout << "Part 1 answer: " << sums[0] << std::endl;
    // FIXME: Should be O(10^6) times larger than part one, some overflow?
    std::cout << "Part 2 answer: " << sums[1] << std::endl;

    return EXIT_SUCCESS;
}