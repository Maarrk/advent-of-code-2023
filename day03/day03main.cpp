#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "day03.h"

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

    std::string line;
    while (std::getline(input_file, line)) {
        std::cout << line << std::endl;
    }

    return EXIT_SUCCESS;
}