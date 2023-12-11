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
    set_usage(mat);

    auto max_distance = std::ranges::max(
        mat.container() | std::views::transform([](Cell c) {
            return c.usage.index() == 1 ? std::get<short>(c.usage) : -1;
        }));
    auto enclosed_count = std::ranges::count_if(mat.container(), [](Cell c) {
        return c.usage.index() == Cell::Contained &&
               std::get<Cell::Contained>(c.usage);
    });

    std::cout << "Part 1 answer: " << max_distance << std::endl;
    std::cout << "Part 2 answer: " << enclosed_count << std::endl;

    return EXIT_SUCCESS;
}