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

    bool part2{false};
    if (argc >= 3 && argv[2][0] == '2')
        part2 = true;

    std::ifstream input_file{input_filename};
    if (input_file.fail()) {
        std::cerr << "Could not open " << input_filename << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    int sum = 0;
    while (std::getline(input_file, line)) {
        const auto is_digit = [](char c) { return c >= '0' && c <= '9'; };

        if (part2)
            mark_spelled_digits(line);

        auto first_it = std::find_if(line.begin(), line.end(), is_digit);
        if (first_it == line.end()) {
            std::cerr << "Line " << line << " did not contain any digits"
                      << std::endl;
        }
        auto last_it = std::find_if(line.rbegin(), line.rend(), is_digit);

        auto tens = *first_it - '0';
        auto ones = *last_it - '0';

        sum += tens * 10 + ones;
    }

    std::cout << "Part " << (part2 ? 2 : 1) << " answer: " << sum << std::endl;

    return EXIT_SUCCESS;
}

void mark_spelled_digits(std::string &text) {
    struct match {
        size_t index{(size_t)-1};
        int digit{0};
    };

    const char spellings[10][6] = {"zero", // skip when replacing
                                   "one",  "two",   "three", "four", "five",
                                   "six",  "seven", "eight", "nine"};

    match first_match{};
    match last_match{};
    for (char digit = 1; digit < 10; digit++) {
        size_t match_start = text.find(spellings[digit]);
        if (match_start != -1 &&
            (first_match.index == -1 || match_start < first_match.index)) {
            first_match.index = match_start;
            first_match.digit = digit;
        }
        match_start = text.rfind(spellings[digit]);
        if (match_start != -1 &&
            (last_match.index == -1 || match_start > last_match.index)) {
            last_match.index = match_start;
            last_match.digit = digit;
        }
    }

    if (first_match.index != -1)
        text[first_match.index] = '0' + first_match.digit;
    if (last_match.index != -1)
        text[last_match.index] = '0' + last_match.digit;
}
