#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

#include "day10.h"

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

    auto mat = load_mat(input_file);
    add_links(mat);
    count_distance(mat);
    auto max_distance =
        std::ranges::max(mat.container() | std::views::transform([](Cell c) {
                             return c.start_distance ? *c.start_distance : -1;
                         }));

    std::cout << "Part 1 answer: " << max_distance << std::endl;
    std::cout << "Part 2 answer: " << 0 << std::endl;

    return EXIT_SUCCESS;
}