#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::string_literals;

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
    int sum_part1 = 0;
    while (std::getline(input_file, line)) {
        auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
        auto first_it = std::find_if(line.begin(), line.end(), is_digit);
        if (first_it == line.end()) {
            std::cerr << "Line " << line << " did not contain any digits"
                      << std::endl;
            return EXIT_FAILURE;
        }
        auto last_it = std::find_if(line.rbegin(), line.rend(), is_digit);

        auto tens = *first_it - '0';
        auto ones = *last_it - '0';

        sum_part1 += tens * 10 + ones;
    }

    std::cout << "Part 1 answer: " << sum_part1 << std::endl;

    return EXIT_SUCCESS;
}