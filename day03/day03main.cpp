#include <algorithm>
#include <charconv>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "day03.h"

using namespace std::literals;

struct GearInfo {
    int part_count{0};
    int part_product{1};
};

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

    auto mat = load_istream(input_file);

    std::cout << std::format("Loaded mat {} rows by {} columns\n",
                             mat.extent(0), mat.extent(1));

    const auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
    const auto not_digit = [](char c) { return c < '0' || c > '9'; };
    std::optional<char> symbol{};
    const auto search_symbol = [&symbol](char c, size_t row, size_t col) {
        if (is_symbol(c)) {
            symbol = c;
            return false;
        }
        return true;
    };

    std::map<std::pair<size_t, size_t>, GearInfo> gears{};

    int parts_sum{0};
    for (size_t row = 0; row < mat.extent(0); row++) {
        auto row_begin = it_at(mat, row, 0);
        auto num_end = row_begin;
        while (true) {
            auto num_begin = std::find_if(num_end, row_end(mat, row), is_digit);
            if (num_begin == row_end(mat, row))
                break;
            num_end = std::find_if(num_begin, row_end(mat, row), not_digit);

            int number;
            auto result = std::from_chars(&(*num_begin), &(*num_end), number);
            if (result.ec != std::errc{}) {
                throw std::runtime_error{
                    std::format("Couldn't convert '{}' to number",
                                std::string{num_begin, num_end})};
            }

            int column = std::distance(row_begin, num_begin);
            mat_part columns{column,
                             column + std::distance(num_begin, num_end) - 1};

            iterate_adjacent(mat, {row, row}, columns, search_symbol);

            const auto save_gears = [&gears, number](char c, size_t row,
                                                     size_t col) {
                if (c == '*') {
                    if (!gears.contains({row, col})) {
                        gears[std::make_pair(row, col)] = GearInfo{};
                    }
                    auto &info = gears.at(std::make_pair(row, col));
                    info.part_count++;
                    info.part_product *= number;
                }
                return true;
            };
            iterate_adjacent(mat, {row, row}, columns, save_gears);

            if (symbol) {
                parts_sum += number;
                // std::cout << std::format("Part {} with symbol {}\n", number,
                //                          *symbol);
            } else {
                // std::cout << std::format("Number {} with no symbols\n",
                // number);
            }
            symbol.reset();
        }
    }

    std::cout << "Part 1 answer: " << parts_sum << std::endl;

    int part_product_sum{0};
    for (auto &&kv : gears) {
        // std::cout << std::format("r {}\tc {}\tN {}\tprod {}\n",
        // kv.first.first,
        //                          kv.first.second, kv.second.part_count,
        //                          kv.second.part_product);
        if (kv.second.part_count == 2) {
            part_product_sum += kv.second.part_product;
        }
    }

    std::cout << "Part 2 answer: " << part_product_sum << std::endl;

    return EXIT_SUCCESS;
}