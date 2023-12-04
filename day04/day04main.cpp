#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "day04.h"

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

    std::vector<int> matches{};
    while (!input_file.eof()) {
        matches.push_back(read_card_matches(input_file));
    }

    // C++20 <ranges> babyyyy :D
    auto points = matches | std::views::transform([](const int n) {
                      return n > 0 ? pow(2, n - 1) : 0;
                  });
    // C++20 didn't require ranges in <numeric> :(
    int point_sum = std::accumulate(std::ranges::begin(points),
                                    std::ranges::end(points), 0);

    auto card_counts = win_card_counts(matches);
    int card_sum = std::accumulate(card_counts.begin(), card_counts.end(), 0);

    std::cout << "Part 1 answer: " << point_sum << std::endl
              << "Part 2 answer: " << card_sum << std::endl;

    return EXIT_SUCCESS;
}