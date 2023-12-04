#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::string_literals;

void mark_spelled_digits(std::string &text);

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
    int sum_part2 = 0;
    while (std::getline(input_file, line)) {
        {
            auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
            auto first_it = std::find_if(line.begin(), line.end(), is_digit);
            if (first_it == line.end()) {
                std::cerr << "Line " << line << " did not contain any digits"
                          << std::endl;
                goto part2; // FIXME: Rework to alternatively run first or
                            // second part
            }
            auto last_it = std::find_if(line.rbegin(), line.rend(), is_digit);

            auto tens = *first_it - '0';
            auto ones = *last_it - '0';

            sum_part1 += tens * 10 + ones;
        }
    part2: {
        auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
        mark_spelled_digits(line);
        auto first_it = std::find_if(line.begin(), line.end(), is_digit);
        if (first_it == line.end()) {
            std::cerr << "Line " << line << " did not contain any digits"
                      << std::endl;
            return EXIT_FAILURE;
        }
        auto last_it = std::find_if(line.rbegin(), line.rend(), is_digit);

        auto tens = *first_it - '0';
        auto ones = *last_it - '0';

        sum_part2 += tens * 10 + ones;
    }
    }

    std::cout << "Part 1 answer: " << sum_part1 << std::endl
              << "Part 2 answer: " << sum_part2 << std::endl;

    return EXIT_SUCCESS;
}

// FIXME: replace words in order they appear in string, not by value
void mark_spelled_digits(std::string &text) {
    const char spellings[10][6] = {"zero", // skip when replacing
                                   "one",  "two",   "three", "four", "five",
                                   "six",  "seven", "eight", "nine"};
    for (char i = 1; i < 10; i++) {
        size_t match_start = text.find(spellings[i]);
        if (match_start != -1)
            text[match_start] = '0' + i;
        match_start = text.rfind(spellings[i]);
        if (match_start != -1)
            text[match_start] = '0' + i;
    }
}
